import os
import pkgutil
from collections import OrderedDict

pkg_dir = os.path.dirname(__file__)
for (module_loader, name, ispkg) in pkgutil.iter_modules([pkg_dir]):
    pkgutil.importlib.import_module('.' + name, __package__)

generators = OrderedDict()
generators["example_generator"] = example_generator.ExampleGenerator()
generators["read_res0_generator"] = read_res0_generator.ReadRes0Generator()
generators["read_res1_generator"] = read_res1_generator.ReadRes1Generator()
generators["read_imp_def_generator"] = read_imp_def_generator.ReadImpDefGenerator()
generators["read_aarch32_generator"] = read_aarch32_generator.ReadAarch32Generator()
generators["settable_res0_generator"] = settable_res0_generator.SettableRes0Generator()
generators["settable_res1_generator"] = settable_res1_generator.SettableRes1Generator()
generators["settable_imp_def_generator"] = settable_imp_def_generator.SettableImpDefGenerator()
generators["undefined_register_generator"] = undefined_register_generator.UndefinedRegisterGenerator()
