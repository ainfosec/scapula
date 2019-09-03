import shoulder

class RegisterHasRES1Filter(shoulder.filter.abstract_filter.AbstractFilter):
    @property
    def description(self):
        return "registers that contain reserved 1 (RES1) fields"

    def do_filter(self, reg):
        for fs in reg.fieldsets:
            for f in fs.fields:
                if f.name == "1":
                    return False

        return True
