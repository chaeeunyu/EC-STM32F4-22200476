#!/usr/bin/env python3
"""
insert_body.py
Doxygen XML의 <location bodyfile/bodystart/bodyend> 정보로 .c 파일에서 함수 본문을 잘라와,
moxygen이 만든 Markdown의 시그니처 코드 블록을 함수 본문(definition) 전체로 교체한다.

Usage:
    python insert_body.py --xml docs/xml --src . docs/api-*.md
    python insert_body.py --xml docs/xml --src . README.md

Options:
    --xml   Doxygen XML 출력 폴더
    --src   Doxygen을 실행한 폴더 (bodyfile 경로의 기준). 보통 프로젝트 루트
    --vars  전역 변수 정의(예: numberFND 초기값)도 함께 넣기
"""
import argparse
import glob
import os
import re
import sys
import xml.etree.ElementTree as ET


def read_lines(path):
    for enc in ("utf-8", "cp949"):
        try:
            with open(path, encoding=enc) as f:
                return f.read().splitlines()
        except UnicodeDecodeError:
            continue
    with open(path, encoding="utf-8", errors="replace") as f:
        return f.read().splitlines()


def collect_bodies(xml_dir, src_root, include_vars):
    """{name: body_text}"""
    kinds = {"function"} | ({"variable"} if include_vars else set())
    bodies = {}
    cache = {}
    for xml_path in glob.glob(os.path.join(xml_dir, "group__*.xml")):
        root = ET.parse(xml_path).getroot()
        for md in root.iter("memberdef"):
            if md.get("kind") not in kinds:
                continue
            name = md.findtext("name")
            loc = md.find("location")
            if name is None or loc is None:
                continue
            bfile = loc.get("bodyfile")
            bstart = int(loc.get("bodystart", "-1"))
            bend = int(loc.get("bodyend", "-1"))
            if not bfile or bstart < 1:
                continue
            if bend < bstart:          # 변수 등 한 줄짜리 정의
                bend = bstart

            path = os.path.join(src_root, bfile)
            if path not in cache:
                if not os.path.exists(path):
                    print(f"[warn] source not found: {path}", file=sys.stderr)
                    cache[path] = None
                else:
                    cache[path] = read_lines(path)
            lines = cache[path]
            if lines is None:
                continue

            s = bstart - 1             # 1-based inclusive -> 0-based slice
            # bodystart가 '{' 줄을 가리키는 경우(Allman style), 시그니처 줄까지 위로 확장
            while s > 0 and lines[s].lstrip().startswith("{"):
                s -= 1
            while s > 0:
                prev = lines[s - 1].strip()
                if (not prev or prev.endswith((";", "}", "*/"))
                        or prev.startswith(("#", "//", "/*", "*"))):
                    break
                s -= 1
            body = lines[s:bend]
            # 공통 들여쓰기 제거
            indent = min((len(l) - len(l.lstrip()) for l in body if l.strip()), default=0)
            body = [l[indent:] for l in body]
            bodies[name] = "\n".join(body).rstrip()
    return bodies


def patch_markdown(md_path, bodies):
    with open(md_path, encoding="utf-8") as f:
        text = f.read()
    count = 0
    for name, body in bodies.items():
        anchor = "{#" + name.lower() + "}"
        pos = text.find(anchor)
        if pos < 0:
            continue
        # 이 섹션의 끝 = 다음 앵커 또는 '---'
        nxt = re.compile(r"\n(\{#|---)").search(text, pos + len(anchor))
        end = nxt.start() if nxt else len(text)
        section = text[pos:end]
        # 섹션 안의 첫 코드 블록(시그니처)을 본문으로 교체
        m = re.search(r"```(?:cpp|c)\n.*?\n```", section, re.S)
        if not m:
            continue
        new_block = "```c\n" + body + "\n```"
        if m.group(0) == new_block:
            continue
        section = section[:m.start()] + new_block + section[m.end():]
        text = text[:pos] + section + text[end:]
        count += 1
    with open(md_path, "w", encoding="utf-8") as f:
        f.write(text)
    print(f"{md_path}: {count} block(s) replaced")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--xml", required=True)
    ap.add_argument("--src", default=".")
    ap.add_argument("--vars", action="store_true")
    ap.add_argument("md", nargs="+")
    a = ap.parse_args()

    bodies = collect_bodies(a.xml, a.src, a.vars)
    if not bodies:
        sys.exit("no bodies found - check --xml / --src")
    for pattern in a.md:
        for p in glob.glob(pattern):
            patch_markdown(p, bodies)


if __name__ == "__main__":
    main()
