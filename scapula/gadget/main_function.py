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
