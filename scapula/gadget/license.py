import shoulder

def write(outfile):
    with open(shoulder.config.license_template_path, "r") as license:
        for line in license:
            outfile.write("// " + line)
    outfile.write("\n")
