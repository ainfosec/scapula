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
        var4 = writer.declare_variable(outfile, "exceptions", reg.size)
        var5 = writer.declare_variable(outfile, "read_exceptions", reg.size)
        var6 = writer.declare_variable(outfile, "write_exceptions", reg.size)

        writer.write_newline(outfile)

        msg = "Reading initial {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.reset_exception_counter(outfile)
        writer.get_register(outfile, reg, var1)
        writer.get_exception_counter(outfile, var4)
        writer.if_statement(outfile, var4 + " > 0")
        writer.assign_variable(outfile, var5, var5 + " + " + var4, indent=1)
        writer.endif(outfile)
        writer.write_newline(outfile)

        msg = "Clearing {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.reset_exception_counter(outfile)
        writer.set_register(outfile, reg, 0x0000000000000000)
        writer.get_exception_counter(outfile, var4)
        writer.if_statement(outfile, var4 + " > 0")
        writer.assign_variable(outfile, var6, var6 + " + " + var4, indent=1)
        writer.endif(outfile)
        writer.write_newline(outfile)

        msg = "Writing 1s to {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.reset_exception_counter(outfile)
        writer.set_register(outfile, reg, 0xFFFFFFFFFFFFFFFF)
        writer.get_exception_counter(outfile, var4)
        writer.if_statement(outfile, var4 + " > 0")
        writer.assign_variable(outfile, var6, var6 + " + " + var4, indent=1)
        writer.endif(outfile)
        writer.write_newline(outfile)

        msg = "Reading {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.reset_exception_counter(outfile)
        writer.get_register(outfile, reg, var2)
        writer.get_exception_counter(outfile, var4)
        writer.if_statement(outfile, var4 + " > 0")
        writer.assign_variable(outfile, var5, var5 + " + " + var4, indent=1)
        writer.endif(outfile)
        writer.write_newline(outfile)

        msg = "Writing 0s to {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.reset_exception_counter(outfile)
        writer.set_register(outfile, reg, 0x0000000000000000)
        writer.get_exception_counter(outfile, var4)
        writer.if_statement(outfile, var4 + " > 0")
        writer.assign_variable(outfile, var6, var6 + " + " + var4, indent=1)
        writer.endif(outfile)
        writer.write_newline(outfile)

        msg = "Reading {r}...".format(r=reg.name)
        writer.print_debug(outfile, msg)
        writer.reset_exception_counter(outfile)
        writer.get_register(outfile, reg, var3)
        writer.get_exception_counter(outfile, var4)
        writer.if_statement(outfile, var4 + " > 0")
        writer.assign_variable(outfile, var5, var5 + " + " + var4, indent=1)
        writer.endif(outfile)
        writer.write_newline(outfile)

        writer.if_statement(outfile, var1 + " == " + var2 + " && " + var2 + " == " + var3)
        msg = "System register {r} is not writable".format(r=reg.name)
        writer.if_statement(outfile, var5 + " > 0 && " + var6 + " > 0", indent=1)
        writer.print_debug(outfile, msg, indent=2)
        sub_msg = "    Unimplemented / Protected: Exceptions on writes and reads"
        writer.print_debug(outfile, sub_msg, indent=2)

        writer.else_if_statement(outfile, var5 + " > 0", indent=1)
        writer.print_warning(outfile, msg, indent=2)
        sub_msg = "    Unreadable: Exceptions only on reads"
        writer.print_warning(outfile, sub_msg, indent=2)

        writer.else_if_statement(outfile, var6 + " > 0", indent=1)
        writer.print_warning(outfile, msg, indent=2)
        sub_msg = "    Unwritable: Exceptions only on writes"
        writer.print_warning(outfile, sub_msg, indent=2)

        writer.else_statement(outfile, indent=1)
        writer.print_warning(outfile, msg, indent=2)
        sub_msg = "    Static: No exceptions on reads or writes"
        writer.print_warning(outfile, sub_msg, indent=2)
        writer.endif(outfile, indent=1)


        writer.else_if_statement(outfile, var2 + " == 0xFFFFFFFFFFFFFFFF && "
                                        + var3 + " == 0x0")
        msg = "System register {r} is fully writable".format(r=reg.name)
        writer.print_warning(outfile, msg, indent=1)

        writer.else_statement(outfile)
        msg = "System register {r} is partially writable".format(r=reg.name)
        writer.print_warning(outfile, msg, indent=1)
        msg = "RES0 Mask: %lx"
        fmt = "~" + var2
        writer.print_warning(outfile, msg, format_str=fmt, indent=1)
        msg = "RES1 Mask: %lx"
        fmt = var3
        writer.print_warning(outfile, msg, format_str=fmt, indent=1)
        writer.endif(outfile)

        writer.write_newline(outfile)
        msg = "    {r}: %lx -> %lx -> %lx".format(r=reg.name)
        fmt = var1 + ", " + var2 + ", " + var3
        writer.print_debug(outfile, msg, format_str=fmt)

        return
