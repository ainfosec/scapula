import shoulder
import scapula.writer as writer
from scapula.generator.scapula_generator import ScapulaGenerator


class ExampleGenerator(ScapulaGenerator):

    def setup(self, regs):
        regs = shoulder.filter.filters["aarch64"].filter_inclusive(regs)
        return regs

    def generate_testcase(self, outfile, reg):
        if reg.is_readable():
            var1 = writer.declare_variable(outfile, "val", reg.size)
            var2 = writer.declare_variable(outfile, "exceptions", reg.size)

            writer.reset_exception_counter(outfile)
            writer.get_register(outfile, reg, var1)
            writer.get_exception_counter(outfile, var2)

            writer.if_statement(outfile, var2 + " > 0")
            msg = "Reading {reg} caused an exception".format(reg=reg.name)
            writer.print_info(outfile, msg, indent=1)

            writer.else_statement(outfile)
            msg = "{reg} = 0x%016x".format(reg=reg.name)
            writer.print_info(outfile, msg, format_str=var1, indent=1)

            writer.endif(outfile)
