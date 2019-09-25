import shoulder

from scapula.filter import filters
from scapula.transform import transforms
from scapula.generator.scapula_generator import ScapulaGenerator
import scapula.writer as writer


class SettableImpDefGenerator(ScapulaGenerator):

    def setup(self, regs):
        regs = shoulder.filter.filters["aarch64"].filter_inclusive(regs)
        regs = filters["register_has_imp_def"].filter_inclusive(regs)

        regs = transforms["only_imp_def"].transform(regs)
        regs = transforms["unique_imp_def"].transform(regs)

        self.verbose = False

        return regs

    def generate_testcase(self, outfile, reg):
        if reg.is_readable() and reg.is_writeable():
            var_name = writer.declare_variable(outfile, "val", reg.size)
            value = 0xffffffffffffffff if reg.size == 64 else 0xffffffff

            for fs_idx, fs in enumerate(reg.fieldsets):
                for f_idx, f in enumerate(fs.fields):
                    writer.set_field(outfile, reg, f, 0)
                    writer.get_field(outfile, reg, f, var_name)

                    writer.if_statement(outfile, var_name + " == 0")

                    writer.set_field(outfile, reg, f, value, indent=1)
                    writer.get_field(outfile, reg, f, var_name, indent=1)

                    writer.if_statement(outfile, var_name + " != 0", indent=1)

                    msg = "IMPLEMENTAITON_DEFINED in " + reg.name + "[%u] is settable"
                    fmt = "{es}_{rname}_{fname}_msb\n".format(
                        es=reg.execution_state if reg.execution_state else "external",
                        rname=reg.name.lower(),
                        fname=f.name.lower()
                    )
                    writer.print_warning(outfile, msg, format_str=fmt, indent=2)

                    writer.endif(outfile, indent=1)
                    writer.endif(outfile)
                    writer.write_newline(outfile)
