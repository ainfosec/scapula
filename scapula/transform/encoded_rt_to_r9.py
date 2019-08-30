import shoulder


class EncodedRtToR9(shoulder.transform.abstract_transform.AbstractTransform):
    @property
    def description(self):
        d = "changing src/dest register for encoded accessors from r0 to r9"
        return d

    def do_transform(self, reg):
        for am in reg.access_mechanisms["mrs_register"]:
            am.rt = 9

        for am in reg.access_mechanisms["msr_register"]:
            am.rt = 9

        return reg
