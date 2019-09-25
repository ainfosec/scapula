import shoulder

from scapula.filter import filters
from scapula.transform import transforms
from scapula.generator.scapula_generator import ScapulaGenerator
import scapula.writer as writer


class ReadRes0Generator(ScapulaGenerator):

    def setup(self, regs):
        regs = shoulder.filter.filters["aarch64"].filter_inclusive(regs)
        regs = filters["register_has_res0"].filter_inclusive(regs)

        regs = transforms["only_res0"].transform(regs)
        regs = transforms["unique_res0"].transform(regs)

        return regs

    def generate_testcase(self, outfile, reg):
        if reg.is_readable():
            var1 = writer.declare_variable(outfile, "val", reg.size)

            for fs_idx, fs in enumerate(reg.fieldsets):
                for f_idx, f in enumerate(fs.fields):
                    writer.get_field(outfile, reg, f, var1)
                    writer.if_statement(outfile, var1 + " != 0")

                    msg = "RES0 field " + reg.name + "." + f.name + " != 0"
                    writer.print_warning(outfile, msg, indent=1)

                    writer.endif(outfile)
                    writer.write_newline(outfile)
