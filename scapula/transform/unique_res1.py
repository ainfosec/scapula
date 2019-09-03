import shoulder

class UniqueRES1Transform(shoulder.transform.abstract_transform.AbstractTransform):
    @property
    def description(self):
        d = "giving unique names per-register to reserved 1 (RES1) fields"
        return d

    def do_transform(self, reg):
        for fs in reg.fieldsets:
            res1_count = 0
            for f in fs.fields:
                if f.name == "1":
                    f.name = "1_" + str(res1_count)
                    res1_count += 1

        return reg
