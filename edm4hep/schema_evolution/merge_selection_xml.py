#!/usr/bin/env python3
"""Script for merging two selection.xml files into one"""

import sys
import xml.etree.ElementTree as ET
import re


def escape_name_content(match):
    """Replace carrets in the regex group match with XML parsing compatible
    representations"""
    name_value = match.group(1)
    escaped_value = name_value.replace("<", "&lt;").replace(">", "&gt;")
    return f'name="{escaped_value}"'


def preserve_cdata_sections(content):
    """Replace CDATA sections with placeholders to preserve them through XML parsing"""
    cdata_sections = []
    placeholder_pattern = "CDATA_PLACEHOLDER_{}"

    def replace_cdata(match):
        cdata_sections.append(match.group(0))
        return placeholder_pattern.format(len(cdata_sections) - 1)

    # Find and replace all CDATA sections with placeholders
    processed_content = re.sub(r"<!\[CDATA\[.*?\]\]>", replace_cdata, content, flags=re.DOTALL)
    return processed_content, cdata_sections


def restore_cdata_sections(content, cdata_sections):
    """Restore CDATA sections from placeholders"""
    placeholder_pattern = "CDATA_PLACEHOLDER_{}"

    for i, cdata_section in enumerate(cdata_sections):
        placeholder = placeholder_pattern.format(i)
        content = content.replace(placeholder, cdata_section)

    return content


def escape_xml_content(content):
    """Escape < and > characters in XML content while preserving XML structure"""
    lines = content.split("\n")
    escaped_lines = []

    for line in lines:
        # Assume that c++ template syntax only appears in class declarations
        # which will have a name="XYX" somewhere. This also assumes that all of
        # these xml tags are on one line!
        if "name=" in line:
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
    gen_content, gen_cdata = preserve_cdata_sections(f.read())

with open(manual_file, "r", encoding="utf-8") as f:
    manual_content, manual_cdata = preserve_cdata_sections(f.read())

# Escape XML content for parsing
gen_content = escape_xml_content(gen_content)
manual_content = escape_xml_content(manual_content)

gen_tree = ET.fromstring(gen_content)
manual_tree = ET.fromstring(manual_content)

gen_selection = gen_tree.find("selection")
manual_selection = manual_tree.find("selection")
for elem in manual_selection:
    gen_selection.append(elem)

ET.indent(gen_tree, space="  ", level=0)
output_content = unescape_xml_output(ET.tostring(gen_tree, encoding="unicode"))

# Restore CDATA sections in the final output
all_cdata = gen_cdata + manual_cdata
output_content = restore_cdata_sections(output_content, all_cdata)

with open(output_file, "w", encoding="utf-8") as f:
    f.write(output_content)
