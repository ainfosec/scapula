import os
import shoulder

from scapula.generator.scapula_generator import ScapulaGenerator
import scapula.writer as writer

class UndefinedInstructionGenerator(ScapulaGenerator):

    def create_undefined_instr(self, op1, op2, crn, crm):
        instr = shoulder.model.armv8a.ARMv8ARegister()

        op0 = 0b01
        rt = 0b1001
        mnemonic = "{o1}, C{cn}, C{cm}, {o2}".format(
                       o1 = str(op1),
                       cn = str(crn),
                       cm = str(crm),
                       o2 = str(op2)
                   )

        instr.name = "s1_{o1}_{cn}_{cm}_{o2}".format(
                       o1 = str(op1),
                       cn = str(crn),
                       cm = str(crm),
                       o2 = str(op2)
                   )

        instr.long_name = instr.name
        instr.execution_state = "aarch64"
        instr.is_internal = True
        instr.is_banked = False
        instr.is_optional = False

        mrs_am = shoulder.model.armv8a.access_mechanism.MRSRegister(
            op0, op1, op2, crn, crm, mnemonic, rt)
        instr.access_mechanisms["mrs_register"].append(mrs_am)

        msr_am = shoulder.model.armv8a.access_mechanism.MSRRegister(
            op0, op1, op2, crn, crm, mnemonic, rt)
        instr.access_mechanisms["msr_register"].append(msr_am)

        return instr

    def setup(self, regs):
        undefined_instructions = []
        for crn_2 in range(0b10):
            crn = (0b1011 | (crn_2 << 2))
            for op1 in range(0b1000):
                for op2 in range(0b1000):
                    for crm in range(0b10000):
                        instr = self.create_undefined_instr(op1, op2, crn, crm)
                        undefined_instructions.append(instr)

        return undefined_instructions

    def generate_testcase(self, outfile, instr):
        mnemonic = instr.access_mechanisms["mrs_register"][0].operand_mnemonic
        var1 = writer.declare_variable(outfile, "exception_count", 32)
        writer.write_newline(outfile)

        writer.reset_exception_counter(outfile)
        msg = "SYS {m}, #0x0 ...".format(m=mnemonic)
        writer.print_debug(outfile, msg)
        writer.execute_sys_instruction(outfile, instr, 0x0)
        writer.get_exception_counter(outfile, var1)
        writer.if_statement(outfile, var1 + " == 0")
        msg = "SYS {m}, #0x0 executed without an exception".format(m=mnemonic)
        writer.print_warning(outfile, msg, indent=1)
        writer.endif(outfile)
        writer.write_newline(outfile)

        writer.reset_exception_counter(outfile)
        msg = "SYS {m}, #0xFFFFFFFFFFFFFFFF ...".format(m=mnemonic)
        writer.print_debug(outfile, msg)
        writer.execute_sys_instruction(outfile, instr, 0xFFFFFFFFFFFFFFFF)
        writer.get_exception_counter(outfile, var1)
        writer.if_statement(outfile, var1 + " == 0")
        msg = "SYS {m}, #0xFFFFFFFFFFFFFFFF executed without an exception".format(m=mnemonic)
        writer.print_warning(outfile, msg, indent=1)
        writer.endif(outfile)
        writer.write_newline(outfile)

        writer.reset_exception_counter(outfile)
        msg = "SYSL {m} ...".format(m=mnemonic)
        writer.print_debug(outfile, msg)
        writer.execute_sysl_instruction(outfile, instr)
        writer.get_exception_counter(outfile, var1)
        writer.if_statement(outfile, var1 + " == 0")
        msg = "SYSL {m} executed without an exception".format(m=mnemonic)
        writer.print_warning(outfile, msg, indent=1)
        writer.endif(outfile)
        writer.write_newline(outfile)

        return
