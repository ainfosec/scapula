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
    """ Properties of the testcase gadget """

    gadget_name: str = "scapula.gadget.testcase"
    """ Name of the gadget these properties apply to """

    count: int = 0
    """ The number of testcases generated by this gadget """

    prefix: str = "test_case"
    """ A string prefix for the generated function name """

    verbose: bool = False
    """ Set to True to generate verbose print statements """

def testcase(decorated):
    """
    A decorator gadget that generates a function to wrap a single test case.
    The functions generated by this gadget have names of the form
    <prefix>_<counter>, where prefix is a decorator argument, and counter is
    incremented by one for each function generated by this gadget.

    Usage:
        @testcase
        function(generator, outfile, ...):
            outfile.write("inside the test case")

    Generates:
        void test_case_0(void)
        {
            inside the test case
        }
    """

    def testcase_decorator(generator, outfile, *args, **kwargs):

        properties = generator.gadgets["scapula.gadget.testcase"]

        testcase_name = properties.prefix + "_" + str(properties.count)
        testcase_call = "void " + testcase_name + "(void)\n"

        outfile.write(testcase_call)
        outfile.write("{\n")

        if properties.verbose == True:
            banner_text = "**********************************************"
            outfile.write("\tBOOTLOADER_PRINT(\"" + banner_text + "\");\n")

            testcase_text = "Running: " + testcase_name
            outfile.write("\tBOOTLOADER_PRINT(\"" + testcase_text + "\");\n")

        decorated(generator, outfile, *args, **kwargs)

        if properties.verbose == True:
            outfile.write("\tBOOTLOADER_PRINT(\"... test complete\");\n")

        outfile.write("}\n\n")

        properties.count = properties.count + 1

    return testcase_decorator