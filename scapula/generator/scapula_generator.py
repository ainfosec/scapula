import os
import shoulder

import scapula.gadget


class ScapulaGenerator(shoulder.generator.abstract_generator.AbstractGenerator):

    def setup(self, registers):
        """ Override this function to perform any generator-specific setup """
        """ tasks before any generation functions are called """
        pass

    def generate_testcase(self, outfile, reg):
        """ Override this function to generate the contents of a single """
        """ testcase for the given register """
        pass

    def generate(self, registers, outpath):
        registers = self.setup(registers)

        # Generate intrinsics specific to this test
        g = shoulder.generator.c_header_generator.CHeaderGenerator()
        g.generate(registers, outpath)

        g_name = str(type(self).__name__)
        outfile_name = g_name + ".h"
        outfile_path = os.path.abspath(os.path.join(outpath, outfile_name))
        outfile_guard = "SCAPULA_" + g_name.upper() + "_H"
        self.gadgets["shoulder.include_guard"].name = outfile_guard
        self.gadgets["scapula.gadget.main_function"].name = g_name + "_run"
        self.gadgets["scapula.gadget.testcase"].prefix = g_name + "_testcase"

        # Generate test cases
        shoulder.logger.info("Generating outputs using generator: " + g_name)
        with open(outfile_path, "w") as outfile:
            self._generate_all(outfile, registers)

    @shoulder.gadget.license
    @shoulder.gadget.include_guard
    def _generate_all(self, outfile, registers):
        outfile.write("#include \"shoulder.h\"\n\n")
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
