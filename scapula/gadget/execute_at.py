#
# Shoulder
# Copyright (C) 2018 Assured Information Security, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

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
