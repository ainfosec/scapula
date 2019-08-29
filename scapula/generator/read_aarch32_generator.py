import shoulder
from dataclasses import dataclass

from scapula.filter import filters
from scapula.transform import transforms
from scapula.generator.scapula_generator import ScapulaGenerator
from scapula.generator.util import *


@dataclass
class ReadAarch32Generator(ScapulaGenerator):

    def setup(self, regs):
        shoulder.config.encoded_functions = False

        self.execution_state = "aarch32"

        for reg in regs:
            if reg.name.lower() == "id_mmfr1":
                regs = [reg]

        return regs

    def generate_testcase(self, outfile, reg):
        read_all_fields(outfile, reg, el=1)
