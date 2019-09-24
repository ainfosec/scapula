import shoulder
from scapula.generator.scapula_generator import ScapulaGenerator
import scapula.writer as writer


class UndefinedRegisterGenerator(ScapulaGenerator):

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
        var1 = writer.declare_variable(outfile, "test1", reg.size)
        var2 = writer.declare_variable(outfile, "test2", reg.size)
        test_value = writer.declare_variable(outfile, "test_value", reg.size,
                                      value=0XDEADBEEFDEADBEEF, const=True)
        writer.write_newline(outfile)

        msg = "Reading {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.get_register(outfile, reg, var1)

        msg = "Writing {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.set_register(outfile, reg, test_value)

        msg = "Reading {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.get_register(outfile, reg, var2)
        writer.write_newline(outfile)

        writer.if_statement(outfile, var1 + " == " + var2)
        msg = "System register {r} is not writable".format(r=reg.name)
        writer.print_debug(outfile, msg, indent=1)

        writer.else_if_statement(outfile, var2 + " == " + test_value)
        msg = "System register {r} is fully writable".format(r=reg.name)
        writer.print_warning(outfile, msg, indent=1)

        writer.else_statement(outfile)
        msg = "System register {r} is partially writable".format(r=reg.name)
        writer.print_warning(outfile, msg, indent=1)
        writer.endif(outfile)
        writer.write_newline(outfile)

        msg = "    {r}: %x -> %x".format(r=reg.name)
        fmt = var1 + ", " + var2
        writer.print_debug(outfile, msg, format_str=fmt)

        return
