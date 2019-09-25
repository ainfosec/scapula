import shoulder

from scapula.generator.scapula_generator import ScapulaGenerator
import scapula.writer as writer


class ReadAarch32Generator(ScapulaGenerator):

    def setup(self, regs):
        shoulder.config.encoded_functions = False

        self.execution_state = "aarch32"

        for reg in regs:
            if reg.name.lower() == "id_mmfr1":
                regs = [reg]

        return regs

    def generate_testcase(self, outfile, reg):
        if reg.is_readable():
            var1 = writer.declare_variable(outfile, "val", reg.size)
            writer.get_register(outfile, reg, var1)
