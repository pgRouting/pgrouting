#!/usr/bin/env python3
# This file is part of the pgRouting project.
# Copyright (c) 2026-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

"""Fallback for documents using Sphinx extlinks roles.

When ``sphinx.ext.extlinks`` is available this script does nothing: the roles
``:bgl:``, ``:issue:``, ``:pr:`` and ``:milestone:`` are resolved by Sphinx.

When it is NOT available, every ``.rst`` file under the directory given as the
first argument is rewritten so that those roles become plain reStructuredText
hyperlinks. This keeps the documentation buildable on Sphinx installations that
lack the extension (e.g. some packager setups) without changing the rendered
output.

Usage: extlinks_fallback.py <documentation-source-dir>
"""
import os
import re
import sys

# If the extension imports, there is nothing to do.
try:
    import sphinx.ext.extlinks  # noqa: F401
    sys.exit(0)
except Exception:
    pass

ROOT = sys.argv[1] if len(sys.argv) > 1 else "."
ISSUE_URL = "https://github.com/pgRouting/pgrouting/issues/"
PULL_URL = "https://github.com/pgRouting/pgrouting/pulls/"
BGL_URL = "https://www.boost.org/doc/libs/latest/libs/graph/doc/html/graph/"


def milestone_url(version):
    return (
        "https://github.com/pgRouting/pgrouting/issues?"
        "utf8=%E2%9C%93&q=milestone%3A%22Release%20" + version + "%22"
    )


def transform(text):
    # :issue:`123`  ->  `Issue #123 <url>`__
    text = re.sub(
        r":issue:`(\d+)`",
        lambda m: "`Issue #%s <%s%s>`__" % (m.group(1), ISSUE_URL, m.group(1)),
        text,
    )
    # :pr:`123`
    text = re.sub(
        r":pr:`(\d+)`",
        lambda m: "`PR #%s <%s%s>`__" % (m.group(1), PULL_URL, m.group(1)),
        text,
    )
    # :milestone:`4.1.0`
    text = re.sub(
        r":milestone:`([^`]+)`",
        lambda m: "`Milestone %s <%s>`__"
        % (m.group(1), milestone_url(m.group(1))),
        text,
    )
    # :bgl:`Title <relative/path.html>`
    text = re.sub(
        r":bgl:`([^`<]*?)\s*<([^`>]+)>`",
        lambda m: "`%s <%s%s>`__"
        % (m.group(1).strip(), BGL_URL, m.group(2).strip()),
        text,
    )
    # :bgl:`relative/path.html`  (no explicit title -> full URL as caption)
    text = re.sub(
        r":bgl:`([^`<>]+)`",
        lambda m: "`%s%s <%s%s>`__"
        % (BGL_URL, m.group(1).strip(), BGL_URL, m.group(1).strip()),
        text,
    )
    return text


changed = 0
for dirpath, _dirs, files in os.walk(ROOT):
    for name in files:
        if not name.endswith(".rst"):
            continue
        path = os.path.join(dirpath, name)
        with open(path, "r", encoding="utf-8") as fh:
            original = fh.read()
        updated = transform(original)
        if updated != original:
            with open(path, "w", encoding="utf-8") as fh:
                fh.write(updated)
            changed += 1

print("extlinks_fallback: %d file(s) rewritten" % changed)
sys.exit(0)
