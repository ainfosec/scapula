import shoulder

from scapula.filter import filters
from scapula.transform import transforms
from scapula.generator.scapula_generator import ScapulaGenerator
import scapula.writer as writer


class ReadImpDefGenerator(ScapulaGenerator):

    def setup(self, regs):
        regs = shoulder.filter.filters["aarch64"].filter_inclusive(regs)
        regs = filters["register_has_imp_def"].filter_inclusive(regs)

        regs = transforms["only_imp_def"].transform(regs)
        regs = transforms["unique_imp_def"].transform(regs)

        if self.verbose:
            self.gadgets["scapula.gadget.testcase"].verbose = True

        return regs

    def generate_testcase(self, outfile, reg):
        if reg.is_readable():
            var1 = writer.declare_variable(outfile, "val", reg.size)

            for fs_idx, fs in enumerate(reg.fieldsets):
                for f_idx, f in enumerate(fs.fields):
                    writer.get_field(outfile, reg, f, var1)

                    msg = "IMPLEMENTATION_DEFINED {r}.{f} = 0x%016x".format(
                        r=reg.name,
                        f=f.name
                    )
                    writer.print_debug(outfile, msg, format_str=var1)
