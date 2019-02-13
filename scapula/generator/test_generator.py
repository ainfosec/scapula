import os

from shoulder.generator.abstract_generator import AbstractGenerator
from shoulder.logger import logger
from scapula import gadget

class TestGenerator(AbstractGenerator):
    def __init__(self):
        logger.debug("Test generator created")

    def generate(self, objects, outpath):
        try:
            outfile_path = os.path.abspath(os.path.join(outpath, "test_program.h"))
            logger.info("Generating SCAPULA test program")
            with open(outfile_path, "w") as outfile:
                gadget.license.write(outfile)

        except Exception as e:
            msg = "{g} failed to generate output {out}: {exception}".format(
                g = str(type(self).__name__),
                out = outpath,
                exception = e
            )
            raise ShoulderGeneratorException(msg)
