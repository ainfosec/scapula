from dataclasses import dataclass
from shoulder.gadget.gadget_properties import GadgetProperties

@dataclass
class properties(GadgetProperties):
    """ Properties of the execute_at gadget """

    gadget_name: str = "scapula.gadget.execute_at"
    """ Name of the gadget these properties apply to """

    exception_level: int = 0
    """ The exception level to switch to (0, 1, 2, 3) """

    # TODO: Would be nice to switch between 64/32bit and secure-world states
    #  execution_state: str = "aarch64"
    #  """ The execution state to switch to (aarch32, aarch64) """
    #
    #  security_state: str = "nonsecure"
    #  """ The security state to switch to (nonsecure, secure) """

def execute_at(decorated):
    """
    A decorator gadget that generates a function to switch the current exception
    level to a new one

    Usage:
        @execute_at
        function(generator, outfile, ...):
            outfile.write("other functions to be executed at EL2")
    """

    def execute_at_decorator(generator, outfile, *args, **kwargs):
        el = generator.gadgets["scapula.gadget.execute_at"].exception_level
        outfile.write("\tswitch_to_el(" + str(el) + ");\n")
        decorated(generator, outfile, *args, **kwargs)
    return execute_at_decorator
