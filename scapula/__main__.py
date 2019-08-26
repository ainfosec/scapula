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

# Main
regs = shoulder.parser.parse_registers(shoulder.config.xml_register_dir)

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
