#!/usr/bin/env python3
"""Script for merging two selection.xml files into one"""

import sys
import xml.etree.ElementTree as ET
import re


def escape_xml_content(content):
    """Escape < and > characters in XML content while preserving XML structure"""
    lines = content.split("\n")
    escaped_lines = []

    for line in lines:
        # Skip XML declaration and root elements
        stripped = line.strip()
        if (
            stripped.startswith("<?xml")
            or stripped.startswith("<lcgdict")
            or stripped.startswith("</lcgdict")
            or stripped.startswith("<selection")
            or stripped.startswith("</selection")
        ):
            escaped_lines.append(line)
        else:
            # For content lines, escape < and > that are not part of XML tags
            # This is a simple approach - escape content within name attributes
            if 'name="' in line:

                def escape_name_content(match):
                    name_value = match.group(1)
                    escaped_value = name_value.replace("<", "&lt;").replace(">", "&gt;")
                    return f'name="{escaped_value}"'

                escaped_lines.append(re.sub(r'name="([^"]*)"', escape_name_content, line))
            else:
                escaped_lines.append(line)

    return "\n".join(escaped_lines)


def unescape_xml_output(content):
    """Unescape the XML content back to original form"""
    return content.replace("&lt;", "<").replace("&gt;", ">")


if len(sys.argv) != 4:
    print("usage: merge_selection_xml.py <podio-gen-xml> <manual-xml> <output-file>")
    sys.exit(1)

podio_gen_file, manual_file, output_file = sys.argv[1:]

with open(podio_gen_file, "r", encoding="utf-8") as f:
    gen_content = escape_xml_content(f.read())
with open(manual_file, "r", encoding="utf-8") as f:
    manual_content = escape_xml_content(f.read())

gen_tree = ET.fromstring(gen_content)
manual_tree = ET.fromstring(manual_content)

# For now merge only the selection parts
gen_selection = gen_tree.find("selection")
manual_selection = manual_tree.find("selection")


for elem in manual_selection:
    gen_selection.append(elem)

ET.indent(gen_tree, space="  ", level=0)
output_content = ET.tostring(gen_tree, encoding="unicode")

# Unescape the content back to original form
output_content = unescape_xml_output(output_content)

with open(output_file, "w", encoding="utf-8") as f:
    f.write(output_content)
