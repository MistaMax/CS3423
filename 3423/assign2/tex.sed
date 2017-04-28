s/\\/\\verb+\\+/g
s/%/\\%/g
s/\^/\\^/g
s/--/-\\hspace{.01cm}-/g
1 s/^\(..*\)$/\\documentstyle[11pt]{article}\n\\begin{document}\n\\begin{center} {\\bf\n&\n} \\end{center}\n\\begin{description}/
3,$ s/^[A-Z][A-Z][A-Z]*/\\item[&] \\hfill \\\\/
$ s/^.*$/&\n\\end{description}\n\\end{document}/
s/^[ \t]*[+-].*$/& \\\\/
