#!/usr/bin/env python3
import re
import sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("inputfile", help="The README file to run on", default="README.md")
parser.add_argument("--check", action='store_true', help='Use if you want to just check the links without actually changing the file')
args = parser.parse_args()

def check_readme_links():
  edm4hep_yaml_content = ""
  with open("edm4hep.yaml") as edm4hep_yaml:
    edm4hep_yaml_content = edm4hep_yaml.readlines()

  readme_content = ""
  with open("README.md", "r") as readme:
    readme_content = readme.readlines()

  new_readme_content = ""

  links_are_ok = True
  for readme_line in readme_content:
    edm4hep_objects = re.findall("\[(.*?)\]\(https:\/\/github.com\/key4hep\/EDM4hep\/blob\/main\/edm4hep\.yaml#L(\d+?)\)", readme_line)
    if edm4hep_objects:
      for edm4hep_object, original_line_number in edm4hep_objects:
        regex = f".*edm4hep::{edm4hep_object} *: *"
        # find in which line it appears in edm4hep.yaml
        edm4hep_yaml_line_numbers_with_match = [str(i + 1) for i, line in enumerate(edm4hep_yaml_content) if re.match(regex, line)]
        if len(edm4hep_yaml_line_numbers_with_match) != 1:
          print(f"Error: failed to replace line number for {edm4hep_object}, either no or several matches were found in edm4hep.yaml with the regex '{regex}'")
          sys.exit(1)
        if edm4hep_yaml_line_numbers_with_match[0] != original_line_number:
          links_are_ok = False
          readme_line = readme_line.replace(f"[{edm4hep_object}](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L{original_line_number})", f"[{edm4hep_object}](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L{edm4hep_yaml_line_numbers_with_match[0]})")
          print(f"{edm4hep_object} is wrongly linked (line {original_line_number} --> {edm4hep_yaml_line_numbers_with_match[0]})")
    new_readme_content += readme_line
  return links_are_ok, new_readme_content

links_are_ok, new_readme_content = check_readme_links()
if links_are_ok:
  print("README.md links are fine, nothing to change")
else:
  if not args.check:
    with open("README.md", "w") as readme:
      readme.write(new_readme_content)
    print("README.md links updated.")
  else:
    print("README.md links should be updated. (Use the scripts/updateReadmeLinks.py script)")
    sys.exit(1)
