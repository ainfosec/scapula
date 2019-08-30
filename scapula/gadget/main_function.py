from dataclasses import dataclass
from shoulder.gadget.gadget_properties import GadgetProperties

@dataclass
class properties(GadgetProperties):
    """ Properties of the main_function gadget """

    gadget_name: str = "scapula.gadget.main_function"
    """ Name of the gadget these properties apply to """

    name: str = "scapula_main"
    """ The name of the function to generate """

def main_function(decorated):
    """
    A decorator gadget that generates a "main" function to be called by
    Scapula OS's testcase loader

    Usage:
        @main_function
        function(generator, outfile, ...):
            outfile.write("the contents of the main function")

    Generates:
        void scapula_main(void)
        {
            the contents of the main function
        }
    """
    def main_function_decorator(generator, outfile, *args, **kwargs):

        properties = generator.gadgets["scapula.gadget.main_function"]

        outfile.write("void " + str(properties.name) + "(void)\n")
        outfile.write("{\n")
        decorated(generator, outfile, *args, **kwargs)
        outfile.write("}\n\n")

    return main_function_decorator
