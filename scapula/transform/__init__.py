import os
import sys
import pkgutil

pkg_dir = os.path.dirname(__file__)
for (module_loader, name, ispkg) in pkgutil.iter_modules([pkg_dir]):
    pkgutil.importlib.import_module('.' + name, __package__)

# -----------------------------------------------------------------------------
# Module interface
# -----------------------------------------------------------------------------

# Usage:
#
# from shoulder.transform import transforms
# registers = transforms["name"].transform(registers)

transforms = {
    "encoded_rt_to_r9": encoded_rt_to_r9.EncodedRtToR9(),
    "only_res0": only_res0.OnlyRes0Transform(),
    "only_res1": only_res1.OnlyRes1Transform(),
    "only_imp_def": only_imp_def.OnlyImpDefTransform(),
    "unique_res0": unique_res0.UniqueRES0Transform(),
    "unique_res1": unique_res1.UniqueRES1Transform(),
    "unique_imp_def": unique_imp_def.UniqueImpDefTransform()
}
