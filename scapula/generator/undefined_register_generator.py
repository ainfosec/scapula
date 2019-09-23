import shoulder
from dataclasses import dataclass

from scapula.generator.scapula_generator import ScapulaGenerator
from scapula.generator.util import *

@dataclass
class UndefinedRegisterGenerator(ScapulaGenerator):

    verbose: bool = True
    """ Set True to generate more verbose test case messages """

    def create_undefined_reg(self, op1, op2, crn, crm):
        reg = shoulder.model.armv8a.ARMv8ARegister()

        op0 = 0b01
        rt = 0b1001
        mnemonic = "s3_{o1}_C{cn}_C{cm}_{o2}".format(
                        o1 = str(bin(op1)),
                        cn = str(bin(crn)),
                        cm = str(bin(crm)),
                        o2 = str(bin(op2))
                   )

        reg.name = mnemonic
        reg.long_name = mnemonic
        reg.execution_state = "aarch64"
        reg.is_internal = True
        reg.is_banked = False
        reg.is_optional = False

        fieldset = shoulder.model.Fieldset(int(reg.size))
        field_name = "IMPLEMENTATION_DEFINED"
        field_msb = 63
        field_lsb = 0
        fieldset.add_field(field_name, field_msb, field_lsb)
        reg.fieldsets.append(fieldset)

        mrs_am = shoulder.model.armv8a.access_mechanism.MRSRegister(
            op0, op1, op2, crn, crm, mnemonic, rt)
        reg.access_mechanisms["mrs_register"].append(mrs_am)

        msr_am = shoulder.model.armv8a.access_mechanism.MSRRegister(
            op0, op1, op2, crn, crm, mnemonic, rt)
        reg.access_mechanisms["msr_register"].append(msr_am)

        return reg


    def setup(self, regs):
        undefined_regs = []
        op0 = 0b01
        rt = 0b1001
        for crn_2 in range(0b10):
            crn = (0b1011 | (crn_2 << 2))
            for op1 in range(0b1000):
                for op2 in range(0b1000):
                    for crm in range(0b10000):
                        reg = self.create_undefined_reg(op1, op2, crn, crm)
                        undefined_regs.append(reg)

        return undefined_regs

    def generate_testcase(self, outfile, reg):
        outfile.write("uint64_t test1 = 0;\n\n")
        outfile.write("uint64_t test2 = 0;\n\n")

        f_getter_name = get_register_func(reg)
        f_setter_name = set_register_func(reg)

        outfile.write("SCAPULA_DEBUG(\"Reading {r}...\");\n".format(r=reg.name))
        outfile.write("test1 = " + f_getter_name + "();\n")
        outfile.write("SCAPULA_DEBUG(\"Writing {r}...\");\n".format(r=reg.name))
        outfile.write(f_setter_name + "(0XDEADBEEFDEADBEEF);\n")
        outfile.write("SCAPULA_DEBUG(\"Reading {r}...\");\n".format(r=reg.name))
        outfile.write("test2 = " + f_getter_name + "();\n")

        outfile.write("if (test1 == test2) SCAPULA_DEBUG(\"NO CHANGE\");\n")
        outfile.write("else if (test2 == 0XDEADBEEF) SCAPULA_ALERT(\"WRITABLE\");\n")
        outfile.write("else SCAPULA_ALERT(\"UNKNOWN BEHAVIOR\");\n")

        outfile.write("SCAPULA_DEBUG(\"    {r}: %x -> %x\", test1, test2);\n".format(
            r = reg.name
        ))

        return
