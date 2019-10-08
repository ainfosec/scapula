import shoulder
import os
import shutil
import copy
import sys

import scapula.generator
from scapula.transform import transforms
from shoulder.cmd_args import parse_cmd_args

# Scapula-specific constants
scapula_pkg_dir = os.path.abspath(os.getcwd())
scapula_output_dir = os.path.join(scapula_pkg_dir, "output")

# Shoulder configs
config = parse_cmd_args(sys.argv[1:])
shoulder.config.shoulder_output_dir = scapula_output_dir

regs = shoulder.parser.parse_registers(shoulder.config.xml_register_dir)

# Run Generators
for key, g in scapula.generator.generators.items():
    sub_outdir = os.path.join(scapula_output_dir, str(g.__class__.__name__))
    sub_include_dir = os.path.join(sub_outdir, "include")

    if os.path.exists(sub_outdir):
        shutil.rmtree(sub_outdir)

    if os.path.exists(shoulder.config.shoulder_include_dir):
        shutil.copytree(shoulder.config.shoulder_include_dir, sub_include_dir)

    # Scaupla uses custom register accessors that need to preserve the contents
    # of r0, so allways use r9 as a source/destitation for encoded accessors
    regs = transforms["encoded_rt_to_r9"].transform(regs)
    shoulder.config.encoded_functions = True

    g.generate(copy.deepcopy(regs), sub_outdir)

# Create a main() function that runs all aarch64 generators
with open(os.path.join(scapula_output_dir, "aarch64_main.c"), "w") as outfile:
    outfile.write("#include <scapula_os/init.h>\n")
    outfile.write("#include <shoulder/CHeaderGenerator/shoulder.h>\n")

    for key, g in scapula.generator.generators.items():
        outfile.write("void " + str(g.__class__.__name__) + "_run();\n")

    outfile.write("\nvoid main(struct scapula_os_config * cfg)\n{\n")
    outfile.write("\tinit_scapula_os(cfg);\n")

    for key, g in scapula.generator.generators.items():
        if g.execution_state == "aarch64":
            outfile.write("\t" + str(g.__class__.__name__) + "_run();\n")
    outfile.write("}")

# Create a main() function that runs all aarch32 generators
with open(os.path.join(scapula_output_dir, "aarch32_main.c"), "w") as outfile:
    outfile.write("\nvoid main()\n{\n")
    # TODO
    outfile.write("}")
