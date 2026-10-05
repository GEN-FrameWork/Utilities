<p align="center">
  <img src="GENIcon.png" alt="GEN FrameWork" width="180"/>
</p>

# ActionScriptQA

**ActionScriptQA** is a GEN FrameWork utility for **automating UI / application tests** through scripts (JavaScript, Lua, or G).

It launches target applications, drives the UI (mouse, keyboard, bitmap find/wait), optionally validates results via GEN `XTrace` feedback (`#[TESTS_RESULT]#`), and writes **CSV evidence** you can open in Excel.

It works best with applications built with **GEN FrameWork** (same scripting libraries, screen helpers, and Trace test IDs), but can also drive other Windows desktop apps when window titles and on-screen bitmaps are stable enough.

---

## What it is for

| Goal | How ActionScriptQA helps |
|------|--------------------------|
| Automate regression UI tests | Scripts click, type, and wait for bitmaps |
| Validate GEN apps under test | TraceServer + test catalog (`*_ListTest.json`) |
| Record new flows quickly | F1 script recorder generates `.js` |
| Keep auditable results | CSV evidence under `assets/Tests/<TestName>/evidences/` |

---

## Layout

```
Utilities/ActionScriptQA/
├── CMake/                      # Build (Ninja / Visual Studio presets)
├── Sources/                    # Application sources
└── assets/
    ├── actionscriptqa.ini      # Configuration (script list, recorder, log, trace)
    └── Tests/                  # XPATHSMANAGERSECTIONTYPE_TESTS
        ├── Tests_UISystem/
        │   ├── Tests_UISystem.js
        │   ├── Tests_UISystem_ListTest.json
        │   ├── graphics/       # Bitmaps for this test only
        │   └── evidences/      # CSV evidence (generated at run time)
        └── Tests_Recorded/     # F1 recorder output (same layout)
            ├── Tests_Recorded.js
            ├── graphics/
            └── evidences/
```

Each test lives in `assets/Tests/<ScriptBase>/` where `<ScriptBase>` is the script name without `.js`. Graphics and evidences for that test stay inside that folder. Working assets are next to the executable via GEN path sections (`Tests/`, etc.).

---

## Build & run

Compile **ActionScriptQA** with the official GEN scripts under `Common/Scripts/compile` (same flow as the GEN Framework website: *Compile from the Common folder*).

Working directory:

```text
Common/Scripts/compile
```

### Windows

```bat
compile.bat actionscriptqa INTEL64 DEBUG COMPILE
```

Other useful variants:

```bat
compile.bat actionscriptqa INTEL32 DEBUG COMPILE
compile.bat actionscriptqa INTEL64 RELEASE COMPILE
compile.bat actionscriptqa INTEL64 DEBUG CMAKE COMPILE
```

### Linux

```bash
./compile.bash actionscriptqa INTEL64 DEBUG COMPILE
```

Other useful variants:

```bash
./compile.bash actionscriptqa INTEL64 RELEASE COMPILE
./compile.bash actionscriptqa INTEL64 DEBUG CMAKE COMPILE
```

Arguments follow the shared GEN compile tool:

| Argument | Meaning |
|----------|---------|
| `actionscriptqa` | Application name from `listapp.txt` |
| `INTEL32` / `INTEL64` / … | Target architecture (host-dependent) |
| `DEBUG` / `RELEASE` | Build type |
| `CMAKE` / `COMPILE` / `TEST` | Stages (omit stages to run the default full pipeline) |

Binary output (example, Debug INTEL64):

```text
Utilities/ActionScriptQA/CMake/Build/<OS>/intel64/debug/actionscriptqa[.exe]
```

Run the executable from that build tree so GEN can resolve `assets/` and the scripts path.

---

## Configuration (`actionscriptqa.ini`)

### Scripts to run

```ini
[scriptslist]
scripts001=Tests_UISystem.js
scripts002=
```

List the script files (resolved under `assets/Tests/<base>/<base>.js`) that ActionScriptQA should execute.

### Script recorder (F1)

```ini
[scriptrecord]
outputscript=Tests_Recorded.js
bitmapprefix=rec_
capturewidth=64
captureheight=32
waittimeoutms=10000
waitintervalms=500
```

| Key | Meaning |
|-----|---------|
| `outputscript` | Generated / appended `.js` under `Tests/<base>/` |
| `bitmapprefix` | Prefix for captured PNG bitmaps |
| `capturewidth` / `captureheight` | Size of click / wait bitmaps |
| `waittimeoutms` / `waitintervalms` | Defaults for `Screen_WaitBitmap` steps |

### Trace (for GEN apps that emit test results)

```ini
[general]
trace_target01=3,*:10001
```

Scripts can open a TraceServer on that UDP port and wait for `#[TESTS_RESULT]#` lines keyed by test ID.

---

## Console keys

| Key | Action |
|-----|--------|
| **Space** | Run the configured script list (`ExecScripts`) |
| **F1** | Start / stop the **script recorder** |
| **F5** | Cancel record mode (if active) and **delete** `assets/Tests/Tests_Recorded/` (all files) |
| **ESC** | Leave record mode (if recording); otherwise exit the application |

---

## Using scripts

1. Put your script in `assets/Tests/<MyFlow>/MyFlow.js` (create `graphics/` and `evidences/` beside it).
2. Optionally add `MyFlow_ListTest.json` next to it (see [Test lists](#test-lists-listtestjson)).
3. Register it in `[scriptslist]` in `actionscriptqa.ini`.
4. Start ActionScriptQA and press **Space** to run.

Scripts use GEN script libraries, including:

- **Screen** — find windows, click by bitmap, `Screen_WaitBitmap`, focus, position  
- **InputSimulate** — mouse / keyboard simulation  
- **Process / System** — launch and terminate applications  
- **Trace / TraceServer** — load catalogs, wait for `#[TESTS_RESULT]#`  
- **FileCSV** — evidence CSV create / append / close  
- **Path** — `GetNameScript()`, `GetPathScript()` for dynamic file names  

Example pattern (names resolve from the running script):

```javascript
var scriptname  = GetNameScript();   // e.g. "Tests_UISystem"
var catalogpath = GetPathScript() + ScriptBaseName(scriptname) + "_ListTest.json";

EvidenceStart(scriptname);           // evidences/<base>_<YYYYMMDD_HHMMSS>.csv
if(IsItExists(catalogpath)) TraceTests_Load(catalogpath);

// ... drive UI, assert, EvidenceAdd(id, description, "PASS"|"FAIL"|"SKIP") ...

if(listloaded) TraceTests_DeleteAll();
EvidenceEnd();
```

See `assets/Tests/Tests_UISystem/Tests_UISystem.js` for a complete GEN UI example.

---

## Creating scripts with the recorder (F1)

The recorder captures real UI interactions and writes / appends a JavaScript file (`outputscript`, default `Tests_Recorded.js`).

### Steps

1. Launch ActionScriptQA (and preferably have the app under test ready or about to start).
2. Press **F1** — record mode ON. Console shows short instructions.
3. **Left-click** a window of the application under test (selects the target app).
4. Interact:
   - **Left-click** a control → click step + bitmap capture  
   - **Right-click** a region → wait/assert step (`Screen_WaitBitmap`) + bitmap  
   - **Type** text → text steps; **Enter / Tab / Backspace** → key steps  
5. Press **F1** or **ESC** again to finish — the script is saved.
6. Press **F5** anytime to discard the current record session (no save) and delete `assets/Tests/Tests_Recorded/` including graphics and evidences.

New sessions append `Recorded_NNN()` functions and refresh `main()`.

### What the generated script includes

- **`EnsureApplication_<app>()`** — one function per captured application (launch if needed, set focus, leave ready for tests). Reuses an already prepared instance within the same script run. If the app is **Chrome / Edge / Firefox**, launch uses `OpenURL(url)` with a placeholder (`https://EDIT_ME/`) that you must edit in the script (URL is not captured).
- **`main()`** calls each `EnsureApplication_<app>()` just before the first `Recorded_NNN()` that targets that app (not inside the tests).
- Replay of click / wait / text / key actions inside `Recorded_NNN()`  
- **Evidence CSV** start / per-step result / close  
- **Optional ListTest load**:  
  `GetPathScript() + <GetNameScript()>_ListTest.json`  
  If the file is missing, the step is recorded as `SKIP` and the script continues.

Bitmaps are stored under `assets/Tests/Tests_Recorded/graphics/` (bound as the GRAPHICS path while recording), with names like `rec_001.png` depending on `bitmapprefix`.

---

## Test lists (`*_ListTest.json`)

A **ListTest** file maps numeric test IDs to descriptions. GEN applications under test emit results with the same IDs (via XTrace / `#[TESTS_RESULT]#`).

### Naming

```
<script base name>_ListTest.json
```

Examples:

| Script | ListTest file |
|--------|----------------|
| `Tests_UISystem.js` | `Tests_UISystem_ListTest.json` |
| `Tests_Recorded.js` | `Tests_Recorded_ListTest.json` |

The base name comes from `GetNameScript()` at runtime (same folder as the script).

### Format

```json
{
  "tests": [
    { "id": 1001, "description": "UI_System: nav Resumen selected" },
    { "id": 1002, "description": "UI_System: nav CPU selected" },
    { "id": 1099, "description": "UI_System: chrome close selected" }
  ]
}
```

### How scripts use it

1. `TraceTests_Load(path)` loads the catalog.  
2. Scripts wait for Trace lines such as `#[TESTS_RESULT]# :,1001,0,...` (error `0` = pass).  
3. Descriptions from the catalog are used in logs and evidence rows.  
4. `TraceTests_DeleteAll()` clears the catalog when finished.

If the ListTest file does **not** exist yet, recorded scripts still run; they only skip loading the catalog.

---

## Evidence CSV (open in Excel)

### Where files are written

```
assets/Tests/<ScriptBase>/evidences/<ScriptBase>_<YYYYMMDD_HHMMSS>.csv
```

Example: `Tests/Tests_UISystem/evidences/Tests_UISystem_20261004_202109.csv`

Separator is **semicolon** (`;`), which Excel (European locales) treats as columns when you open or import the file.

### Columns

| Column | Content |
|--------|---------|
| **ID** | Test / step ID (integer), or empty for setup steps |
| **Description** | Human-readable step or catalog description |
| **Result** | `PASS`, `FAIL`, or `SKIP` |

### Example

```csv
ID;Description;Result
;TraceTests_Load;PASS
;TraceServer_Ini;PASS
;ExecApplication;PASS
;WaitWindow;PASS
1001;UI_System: nav Resumen selected;PASS
1002;UI_System: nav CPU selected;PASS
1099;UI_System: chrome close selected;PASS
```

### Viewing in Microsoft Excel

1. Open Excel.  
2. **Data → From Text/CSV** (or double-click the `.csv` if your locale uses `;` as list separator).  
3. Choose file type **Delimited**, delimiter **Semicolon**.  
4. Confirm that columns **ID**, **Description**, and **Result** appear as three separate columns.  
5. Filter or pivot on **Result** to see failures quickly.

Tip: do not rely on Notepad alone for review — Excel (or LibreOffice Calc with semicolon delimiter) shows the table structure clearly.

---

## Recommended workflow

1. **Record** a flow with F1 against the application under test.  
2. Add or refine bitmaps in that test's `graphics/` folder, and waits if timing is flaky.  
3. If the app is GEN-based, create `<script>_ListTest.json` with the same IDs the app emits (next to the `.js`).  
4. Register the script in `actionscriptqa.ini` and run with **Space**.  
5. Open the latest file under `Tests/<ScriptBase>/evidences/` in Excel to review PASS / FAIL.

---

## Related GEN pieces

- Script libraries: `GEN/Script/Lib` (`Script_Lib_Screen`, `Script_Lib_FileCSV`, `Script_Lib_Trace`, …)  
- CSV backend: `GEN/XUtils/FormatFiles/XFileCSV`  
- Example GEN UI under test: `Examples/Graphics/UI_System`

---

## License

Same terms as the GEN FrameWork project (see repository root).
