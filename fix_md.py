#!/usr/bin/env python3
r"""
fix_md.py - moxygen 출력 Markdown을 GitHub 렌더링에 맞게 후처리

1. Description 칸이 전부 비어 있는 표 -> 값 목록 한 줄로 변환
2. :::note ... :::  -> > [!NOTE] (GitHub alert)
3. {#anchor}        -> <a id="anchor"></a>
4. 코드 밖의 ~       -> \~  (GitHub에서 ~...~ 가 취소선(strikethrough)이 되는 것 방지)

사용법:
    python fix_md.py README.md                     # 이미 있는 파일을 고침
    python fix_md.py README.md "api-*.md"
    python fix_md.py --merge README.md intro.md api-PinNames.md ...
        # 여러 md를 UTF-8로 합쳐서 README.md를 만든 뒤 고침
        # (PowerShell Get-Content/Set-Content 대신 사용 -> 한글 깨짐 없음)
여러 번 실행해도 결과가 같다 (idempotent).
"""
import glob
import re
import sys

# Docusaurus admonition type -> GitHub alert type
ADMONITION = {
    "note": "NOTE", "info": "NOTE", "tip": "TIP", "important": "IMPORTANT",
    "warning": "WARNING", "caution": "CAUTION", "danger": "CAUTION",
}


# ---------- 3. {#anchor} ----------
def fix_anchors(lines):
    out = []
    for line in lines:
        m = re.fullmatch(r"\s*\{#([\w\-]+)\}\s*", line)
        out.append(f'<a id="{m.group(1)}"></a>' if m else line)
    return out


# ---------- 2. :::note ... ::: ----------
def fix_admonitions(lines):
    out, i = [], 0
    while i < len(lines):
        m = re.fullmatch(r"\s*:::(\w+)\s*", lines[i])
        if m:
            # 닫는 ::: 찾기 (없으면 손대지 않음)
            j = i + 1
            while j < len(lines) and lines[j].strip() != ":::":
                j += 1
            if j < len(lines):
                kind = ADMONITION.get(m.group(1).lower(), "NOTE")
                body = [b.rstrip() for b in lines[i + 1:j]]
                while body and not body[0]:
                    body.pop(0)
                while body and not body[-1]:
                    body.pop()
                out.append(f"> [!{kind}]")
                out += [f"> {b}" if b else ">" for b in body]
                out.append("")
                i = j + 1
                continue
        out.append(lines[i])
        i += 1
    return out


# ---------- 1. Description이 빈 표 ----------
def _cells(line):
    s = line.strip()
    if s.startswith("|"):
        s = s[1:]
    if s.endswith("|"):
        s = s[:-1]
    return [c.strip() for c in s.split("|")]


def _is_separator(line):
    return re.fullmatch(r"\s*\|?[\s:\-|]+\|?\s*", line) is not None and "-" in line


def fix_empty_tables(lines):
    out, i = [], 0
    while i < len(lines):
        if lines[i].lstrip().startswith("|"):
            j = i
            while j < len(lines) and lines[j].lstrip().startswith("|"):
                j += 1
            block = lines[i:j]
            header = _cells(block[0])
            if (len(block) >= 3 and _is_separator(block[1])
                    and header[-1] == "Description"
                    and all(_cells(r)[-1] == "" for r in block[2:])):
                values = [_cells(r)[0] for r in block[2:]]
                out.append(" · ".join(values))   # 표 대신 값 목록 한 줄
                out.append("")
            else:
                out += block
            i = j
            continue
        out.append(lines[i])
        i += 1
    return out


# ---------- 4. ~ 취소선 방지 ----------
# 코드 블록(```), inline code(`...`), 링크 주소(](...)) 안은 건드리지 않음
_PROTECT = re.compile(r"(`[^`]*`|\]\([^)]*\))")


def escape_tildes(lines):
    out, in_fence = [], False
    for line in lines:
        if re.match(r"\s*(```|~~~)", line):
            in_fence = not in_fence
            out.append(line)
            continue
        if in_fence:
            out.append(line)
            continue
        parts = _PROTECT.split(line)
        for k in range(0, len(parts), 2):          # 짝수 index = 보호 구간 밖
            parts[k] = re.sub(r"(?<!\\)~", r"\\~", parts[k])
        out.append("".join(parts))
    return out


def process(text):
    lines = text.splitlines()
    lines = fix_anchors(lines)
    lines = fix_admonitions(lines)
    lines = fix_empty_tables(lines)
    lines = escape_tildes(lines)
    return lines


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)

    args = sys.argv[1:]

    # --merge OUT IN1 IN2 ... : UTF-8로 읽어서 합치기
    if args[0] == "--merge":
        if len(args) < 3:
            print(__doc__)
            sys.exit(1)
        out_path, inputs = args[1], args[2:]
        chunks = []
        for path in inputs:
            with open(path, encoding="utf-8-sig", errors="surrogateescape") as f:
                chunks.append(f.read().rstrip("\r\n"))
        with open(out_path, "w", encoding="utf-8", errors="surrogateescape", newline="\n") as f:
            f.write("\n\n".join(chunks) + "\n")
        print(f"merged: {len(inputs)} files -> {out_path}")
        args = [out_path]

    # Windows PowerShell/cmd는 *를 펼쳐주지 않으므로 직접 glob
    files = []
    for pattern in args:
        files += glob.glob(pattern) or [pattern]

    for path in files:
        with open(path, encoding="utf-8", errors="surrogateescape", newline="") as f:
            text = f.read()
        eol = "\r\n" if "\r\n" in text else "\n"
        new = eol.join(process(text)) + eol
        if new != text:
            with open(path, "w", encoding="utf-8", errors="surrogateescape", newline="") as f:
                f.write(new)
            print(f"fixed : {path}")
        else:
            print(f"ok    : {path}")


if __name__ == "__main__":
    main()
