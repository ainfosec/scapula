import shoulder

class RegisterHasRES0Filter(shoulder.filter.abstract_filter.AbstractFilter):
    @property
    def description(self):
        return "registers that contain reserved 0 (RES0) fields"

    def do_filter(self, reg):
        for fs in reg.fieldsets:
            for f in fs.fields:
                if f.name == "0":
                    return False

        return True
