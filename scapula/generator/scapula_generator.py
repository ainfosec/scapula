import os
import shoulder
from dataclasses import dataclass

import scapula.gadget


@dataclass
class ScapulaGenerator(shoulder.generator.abstract_generator.AbstractGenerator):

    execution_state: str = "aarch64"
    """ The target execution state for generated test cases (aarch32/aarch64)"""

    verbose: bool = True
    """ Set True to generate more verbose test cases messages """

    def setup(self, registers):
        """ Override this function to perform any generator-specific setup """
        """ tasks before any generation functions are called """
        pass

    def generate_testcase(self, outfile, reg):
        """ Override this function to generate the contents of a single """
        """ testcase for the given register """
        pass

    def generate(self, registers, outpath):

        opath = os.path.abspath(outpath)

        if self.verbose:
            self.gadgets["scapula.gadget.testcase"].verbose = True

        registers = self.setup(registers)

        # Generate intrinsics specific to this test
        shoulder_header_path = os.path.join(opath, "include")
        g = shoulder.generator.c_header_generator.CHeaderGenerator()
        g.generate(registers, shoulder_header_path)

        if self.execution_state == "aarch64":
            opath = os.path.join(opath, "aarch64")
        elif self.execution_state == "aarch32":
            opath = os.path.join(opath, "aarch32")

        os.makedirs(opath, exist_ok=True)

        g_name = str(type(self).__name__)
        outfile_name = g_name + ".c"
        outfile_path = os.path.join(opath, outfile_name)
        self.gadgets["scapula.gadget.main_function"].name = g_name + "_run"
        self.gadgets["scapula.gadget.testcase"].prefix = g_name + "_testcase"

        # Generate test cases
        shoulder.logger.info("Generating outputs using generator: " + g_name)
        with open(outfile_path, "w") as outfile:
            self._generate_all(outfile, registers)

    @shoulder.gadget.license
    def _generate_all(self, outfile, registers):
        outfile.write("#include \"../include/shoulder.h\"\n")
        outfile.write("#include <bootloader/bootloader.h>\n")
        outfile.write("#include <bootloader/bootloader_print.h>\n")
        outfile.write("#include <bootloader/switch_exception_level.h>\n\n")

        self._generate_test_cases(outfile, registers)
        self._generate_main(outfile, registers)

    def _generate_test_cases(self, outfile, registers):
        for reg in registers:
            self._generate_testcase(outfile, reg)

    @scapula.gadget.testcase
    def _generate_testcase(self, outfile, reg):
        self.generate_testcase(outfile, reg)

    @scapula.gadget.main_function
    def _generate_main(self, outfile, registers):
        for i in range(0, self.gadgets["scapula.gadget.testcase"].count):
            prefix = self.gadgets["scapula.gadget.testcase"].prefix
            outfile.write("\t" + prefix + "_" + str(i) + "();\n")
