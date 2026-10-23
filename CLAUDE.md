# CLAUDE.md

Application_Syarikat_Seteguh: weighbridge and trading program (VC6 / MFC, wlib32) for Syarikat Seteguh, with receiving, despatch, invoices, e-invoice and payroll.

Module reference: [docs/MODULE_REFERENCE.md](docs/MODULE_REFERENCE.md) (Part 1 business view, Part 2 technical view, last section is the checklist for `Application_Nibong_Jaya`).

**Rule:** Any change to behaviour, tables, fields, screens, reports or calculations must update the module reference (English, plus zh-CN when the changed section exists there) in the same commit.

Notes for working here:

- The real build is `menu.dsp` (39 `.cpp` files); 16 other `.cpp` files in the folder are not built.
- The menu is `IDR_MAINMENU` in `MENU.RC`; every item is dispatched by `CMenuApp::SubProgram` in `MAIN.CPP`.
- No SQL scripts or schema repository exist for this program: check the database before you assume a column name or type.
- Never put passwords or connection secrets into the docs; name the file and line instead.
