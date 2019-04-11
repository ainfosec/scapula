import os
import shoulder

from scapula.gadget import *
from scapula.filter import filters
from scapula.transform import transforms

class ReadImpDefGenerator(shoulder.generator.abstract_generator.AbstractGenerator):
    def generate(self, objects, outpath):
        # Gadget settings
        self.gadgets["scapula.gadget.testcase"].verbose = False
        self.gadgets["scapula.gadget.testcase"].prefix = "read_imp_def_test"

        # Filters
        regs = objects
        regs = shoulder.filter.filter_all(regs)

        regs = shoulder.transform.transforms["n_counter_to_zero"].transform(regs)
        regs = shoulder.transform.transforms["quirks"].transform(regs)
        regs = shoulder.transform.transforms["special_to_underscore"].transform(regs)

        regs = transforms["only_imp_def"].transform(regs)
        regs = transforms["unique_imp_def"].transform(regs)

        # Generate intrinsics specific to this test
        intrinsics_path = os.path.abspath(os.path.join(outpath, "shoulder.h"))
        g = shoulder.generator.c_header_generator.CHeaderGenerator()
        g.generate(regs, outpath)

        # Generate this test
        outfile_path = os.path.abspath(os.path.join(outpath, "read_imp_def_tests.h"))
        shoulder.logger.info("Generating IMPLEMENTATION_DEFINED field tests")
        with open(outfile_path, "w") as outfile:
            self._generate_all(outfile, regs)

    @shoulder.gadget.license
    @shoulder.gadget.include_guard(name="SCAPULA_IMP_DEF_TESTS_H")
    def _generate_all(self, outfile, objects):
        outfile.write("#include \"shoulder.h\"\n\n")
        self._generate_test_cases(outfile, objects)
        self._generate_main(outfile, objects)

    def _generate_test_cases(self, outfile, objects):
        for o in objects:
            self.gadgets["scapula.gadget.execute_at"].exception_level = 2
            self._read_reserved_fields(outfile, o)

    @main_function
    def _generate_main(self, outfile, objects):
        for i in range(0, self.gadgets["scapula.gadget.testcase"].count):
            prefix = self.gadgets["scapula.gadget.testcase"].prefix
            outfile.write("\t" + prefix + "_" + str(i) + "();\n")

    @testcase
    @execute_at
    def _read_reserved_fields(self, outfile, reg):
        reg_name = str(reg.name).lower()
        current_el = self.gadgets["scapula.gadget.execute_at"].exception_level
        outfile.write("\tuint64_t val = 0;\n")

        for fs_idx, fs in enumerate(reg.fieldsets):
            for f in fs.fields:
                f_name = f.name.lower()
                function_name = "aarch64_"
                function_name += reg_name
                if len(reg.fieldsets) > 1:
                    function_name += "_fieldset_" + str(fs_idx + 1)
                function_name += "_" + f_name
                if f.lsb == f.msb:
                    function_name += "_is_enabled"
                else:
                    function_name += "_get"

                #  info = "Reading " + function_name + " from EL" + str(current_el)
                #  outfile.write("\tBOOTLOADER_PRINT(\"" + info + "\");\n")
                outfile.write("\tval = " + str(function_name) + "();\n")
                outfile.write("\tif(val != 0) {")
                outfile.write("BOOTLOADER_PRINT(\"" + function_name + " RES0 != 0\"); }\n")
