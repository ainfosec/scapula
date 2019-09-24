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
            writer.get_register(outfile, reg, var1)

            msg = "{reg} = 0x%016x".format(reg=reg.name)
            writer.print_info(outfile, msg, format_str=var1)
