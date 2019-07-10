import shoulder
from dataclasses import dataclass

from scapula.filter import filters
from scapula.transform import transforms
from scapula.generator.scapula_generator import ScapulaGenerator
from scapula.generator.util import *


@dataclass
class ReadRes1Generator(ScapulaGenerator):

    verbose: bool = True
    """ Set True to generate more verbose test cases messages """

    def setup(self, regs):
        regs = shoulder.filter.filters["aarch64"].filter_inclusive(regs)
        regs = filters["register_has_res1"].filter_inclusive(regs)

        regs = transforms["only_res1"].transform(regs)
        regs = transforms["unique_res1"].transform(regs)

        if self.verbose:
            self.gadgets["scapula.gadget.testcase"].verbose = True

        return regs

    def generate_testcase(self, outfile, reg):
        if reg.is_readable():
            read_all_fields(outfile, reg, el=2)
