"""Embed the browser preview in flash and enable commands in the served copy."""
from pathlib import Path

folder = Path(__file__).resolve().parent
html = (folder / "controller.html").read_text(encoding="utf-8")
preview_flag = "const live = false;"
if html.count(preview_flag) != 1 or ')controller"' in html:
    raise ValueError("Cannot embed controller.html. Check the preview flag and raw string delimiter.")

live_html = html.replace(preview_flag, "const live = true;")
header = (
    "// Generated from controller.html by embed_controller.py.\n"
    '#pragma once\n\nconst char INDEX_HTML[] PROGMEM = R"controller(\n'
    + live_html
    + ')controller";\n'
)
(folder / "controller_page.h").write_text(header, encoding="utf-8")
