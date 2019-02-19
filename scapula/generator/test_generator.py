import os
import shoulder
import scapula.gadget

class TestGenerator(shoulder.generator.abstract_generator.AbstractGenerator):

    def generate(self, objects, outpath):
        try:
            outfile_path = os.path.abspath(os.path.join(outpath, "test_program.h"))
            shoulder.logger.info("Generating SCAPULA test program")

            with open(outfile_path, "w") as outfile:
                shoulder.gadget.license.generate(objects, outfile)
                shoulder.gadget.include_guard_open.generate(objects, outfile)

                scapula.gadget.switch_to_el1.generate(objects, outfile)

                shoulder.gadget.include_guard_close.generate(objects, outfile)

        except Exception as e:
            msg = "{g} failed to generate output {out}: {exception}".format(
                g = str(type(self).__name__),
                out = outpath,
                exception = e
            )
            raise ShoulderGeneratorException(msg)
