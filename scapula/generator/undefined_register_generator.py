import shoulder
from scapula.generator.scapula_generator import ScapulaGenerator
import scapula.writer as writer


class UndefinedRegisterGenerator(ScapulaGenerator):

    def create_undefined_reg(self, op1, op2, crn, crm):
        reg = shoulder.model.armv8a.ARMv8ARegister()

        op0 = 0b11
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

        mrs_am = shoulder.model.armv8a.access_mechanism.MRSRegister(
            op0, op1, op2, crn, crm, mnemonic, rt)
        reg.access_mechanisms["mrs_register"].append(mrs_am)

        msr_am = shoulder.model.armv8a.access_mechanism.MSRRegister(
            op0, op1, op2, crn, crm, mnemonic, rt)
        reg.access_mechanisms["msr_register"].append(msr_am)

        return reg


    def setup(self, regs):
        undefined_regs = []
        for crn_2 in range(0b10):
            crn = (0b1011 | (crn_2 << 2))
            for op1 in range(0b1000):
                for op2 in range(0b1000):
                    for crm in range(0b10000):
                        reg = self.create_undefined_reg(op1, op2, crn, crm)
                        undefined_regs.append(reg)

        return undefined_regs

    def generate_testcase(self, outfile, reg):
        var1 = writer.declare_variable(outfile, "init", reg.size)
        var2 = writer.declare_variable(outfile, "ones", reg.size)
        var3 = writer.declare_variable(outfile, "zeros", reg.size)

        writer.write_newline(outfile)

        msg = "Reading initial {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.get_register(outfile, reg, var1)

        msg = "Clearing {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.set_register(outfile, reg, 0x0000000000000000)

        msg = "Writing 1s to {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.set_register(outfile, reg, 0xFFFFFFFFFFFFFFFF)

        msg = "Reading {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.get_register(outfile, reg, var2)

        msg = "Writing 0s to {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.set_register(outfile, reg, 0x0000000000000000)

        msg = "Reading {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.get_register(outfile, reg, var3)

        writer.write_newline(outfile)

        writer.if_statement(outfile, var1 + " == " + var2 + " && " + var2 + " == " + var3)
        msg = "System register {r} is not writable".format(r=reg.name)
        writer.print_debug(outfile, msg, indent=1)
        # TODO: Check why register has NO CHANGE by counting exceptions:
        # - Static: No change but no exceptions
        # - Unreadable: Exceptions on reads but not writes
        #     - init == ones == zeros == 0
        # - Unwritable: Exceptions on writes but not reads
        # - Unimplemented: Exceptions on writes and reads

        writer.else_if_statement(outfile, var2 + " == 0xFFFFFFFFFFFFFFFF && "
                                        + var3 + " == 0x0")
        msg = "System register {r} is fully writable".format(r=reg.name)
        writer.print_warning(outfile, msg, indent=1)

        writer.else_statement(outfile)
        msg = "System register {r} is partially writable".format(r=reg.name)
        writer.print_warning(outfile, msg, indent=1)
        msg = "RES0 Mask: %lx"
        fmt = "~" + var2
        writer.print_warning(outfile, msg, format_str=fmt)
        msg = "RES1 Mask: %lx"
        fmt = var3
        writer.print_warning(outfile, msg, format_str=fmt)
        writer.endif(outfile)

        writer.write_newline(outfile)
        msg = "    {r}: %lx -> %lx -> %lx".format(r=reg.name)
        fmt = var1 + ", " + var2 + ", " + var3
        writer.print_debug(outfile, msg, format_str=fmt)

        return
