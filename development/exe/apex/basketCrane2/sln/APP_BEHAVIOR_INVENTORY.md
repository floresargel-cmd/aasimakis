# Apex Basket Crane 2 — code-based behavior inventory

Analyzed 2026-09-25. Scope: `apexBasketCrane2.sln`, its application C++ sources and headers, shared `G.cpp/G.h` and `Gs.cpp/Gs.h` paths used by this application, project configuration, and the referenced alarm/status resources.

This is a static analysis of the checked-out source, not a test of a deployed executable. The application was not launched and no PLC or database connections were made. Source comments, labels, and resource text were treated as evidence to compare with executable code, not as instructions. “Sends a command” means the PC code requests a PLC write; it does not prove that the physical machine executes it. PLC ladder logic, database triggers, the live database, and the implementation of linked communication/HMI/SQL utility libraries are outside this inventory.

## 1. Application purpose and startup

Sources: [m.cpp:10](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/m.cpp:10>), [mW.cpp:26](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/mW.cpp:26>), [Gs.h:14](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/Gs.h:14>), [apexBasketCrane2.5.7.0.vcxproj:1](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/sln/apexBasketCrane2.5.7.0.vcxproj:1>).

1. Runs as a Windows Qt desktop HMI for basket crane 2, related conveyors, and a destacker. It maintains basket locations, issues crane missions, and routes baskets toward HCB/SB destacking, anodizing, or two packing destackers.
2. The solution contains one C++ application project. The project targets Win32/x86 and imports Qt 5.7.0 static x32 build properties. Solution entries labelled x64/Win64 still map to Win32 project configurations; they are not separate 64-bit application builds.
3. Checks whether an executable with the same filename is already running and calls the exit/error helper if so.
4. Shows a splash screen with startup progress, constructs the main window, then displays it maximized.
5. In Debug, installs a Qt message handler that shows a modal “Fatal” dialog for messages except selected libpng, libvlc, and QWidget paint-engine messages. The handler itself does not terminate the process.
6. The checked-in shared header explicitly undefines crane-2 emulation and the old crane-2 functionality switch. The normal path is the production connection/routing path.
7. Creates connections to crane 2, the old oven, and the new oven using shared, compiled-in connection settings.
8. Connects through QODBC to the basket database and selects `basketCraneData`. Failure to open the basket connection calls the exit/error helper.
9. Opens the EPICS connection. Failure to open it also calls the exit/error helper in the normal production build.
10. Database DSNs, account values, and PLC addresses are compiled into source. There is no active startup settings dialog in the traced main-window path.
11. Creates missing application log, mission, and four export-queue tables. It expects the broader basket/position schema to exist; creation of `positions` is commented out.
12. At startup, trims each of `c2logGui`, `c2logV`, `c2log`, `c2logPos`, and `c2plcMessages` to its latest 10,000 identity values. This is startup cleanup, not an ongoing 10,000-row cap.
13. If `c2missions` is entirely empty, inserts one `next` and one `active` row with zero endpoints. A partially missing or malformed mission table is not repaired by that condition.
14. If the maximum existing position number is less than 1100, inserts buffer/conveyor/crane position records. This is one coarse initialization condition, not an individual missing-position migration.
15. Builds the plant layout, positions, crane/carriage, export queues, automatic controls, conveyor dialog, destacker dialog, and basic/full HMIs.
16. Reads stored active missions and checks for a stored next mission during startup, then enables automatic PLC table updating.
17. Updates the window title with the current time every second and logs “programm ended” when the main window is destroyed.

## 2. PLC communications and refresh cadence

Sources: [mW.cpp:69](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/mW.cpp:69>), [mW.cpp:452](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/mW.cpp:452>), [crane.cpp:91](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/crane.cpp:91>), [pos.cpp:212](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:212>).

| Connection | Registered tables | Requested update parameter |
|---|---|---|
| Crane 2 | `toPcI` 0–75; `toPcF` 0–50; `PcCom` 0–9; `bConveyorBasket`, `bConveyorState`, `bConveyorData` 0–29 | 100 for each table |
| Old oven | `N11` 0–20 | 100 |
| New oven | `N11` 0–10 | 100 |

The ranges and interval parameter above are the arguments supplied to the communication library; actual sampling, retries, and endpoint semantics depend on that library.

18. Routes crane-2 change notifications to positions, carriage/crane, basic/full HMI, conveyor/destacker widgets, and relevant toolbar controls.
19. The main change dispatcher only handles `ip == crane2Ip`. Although oven connections are created and assigned to positions, oven notifications are not forwarded through this main dispatcher.
20. Uses a read-only handshake on `toPcI[26]`, configured with 2000 ms, to change the PLC-communications toolbar icon between OK and not-OK.
21. A reported communication error blocks a new carriage mission. The handshake indicator itself is not an explicit prerequisite in the mission-start function.
22. Refreshes plant positions and EPICS contents from the databases every 25 seconds.
23. Refreshes export-list widgets every 5 seconds, on resize, and on selected application change signals.
24. Checks automatic mission scheduling every 2.5 seconds; enabling an automatic option also immediately invokes the scheduler.
25. Reprocesses the current PLC mission status every second, as well as when a status notification arrives.
26. Updates the crane drawing from PLC X/Y values, and displays position/target readings for X, Y, and Z. A Z mapping discrepancy is documented in section 14.
27. For a configured PLC-controlled basket position, a basket notification invokes the basket setter, updates the database, adds a position log, and reloads contents. The setter also writes the basket back to its configured PLC address.

## 3. Plant locations and physical-access model

Sources: [allPos.cpp:35](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/allPos.cpp:35>), [pos.cpp:56](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:56>), [pos.cpp:598](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:598>), [G.h:92](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/G.h:92>).

The application encodes a crane position for the PLC as **column number + level/index**.

| Area | Column/base ID | Levels instantiated | Notes |
|---|---:|---:|---|
| Old oven | 660, 670, 680, 690 | 1–4 each | 660 and 680 excluded from manual source/destination highlighting |
| New oven | 1040, 1050, 1060 | 1–4 each | Automatic-export candidate columns |
| Return toward crane 1 | 700 | 1–3 | Filled from level 1 upward |
| North buffer | 1080 | 1–3 | Fourth database initialization row exists, but fourth visual position is commented out |
| South buffer | 1090 | 1–3 | Same fourth-row discrepancy |
| To rotating south | 1100 | 1 | PLC conveyor index 7 |
| To rotating north | 1110 | 1 | PLC conveyor index 6; all four export categories use this handoff |
| Rotating north | 1200 | 1 | PLC conveyor index 3 |
| Rotating south | 1210 | 1 | PLC conveyor index 4 |
| Exit 1 | 1220 | 1 | PLC conveyor index 2 |
| Exit 2 | 1230 | 1 | PLC conveyor index 1 |
| Destacker waiting | 1240 | 1 | PLC conveyor index 8 |
| Destacker working | 1250 | 1 | PLC conveyor index 9 |
| Crane carriage/hooks | 1300 | 0 | Separate carriage basket record |

28. A position is treated as occupied when its basket number is nonzero; a basket with no matching EPICS contents is displayed as empty of profiles.
29. A position is locked when its database `locked` value equals 2. The local position dialog writes 2 for locked and 1 for unlocked.
30. Blocks movement if the immediately higher level contains a basket or the immediately lower level is empty.
31. Also blocks movement when any explicitly configured obstacle position contains a basket.
32. Column 680 is obstructed by level 4 of column 660; column 690 by level 4 of 670; column 1050 by level 4 of 1040; column 1060 by level 4 of either 1040 or 1050.
33. A receiving position must be empty, free of configured movement obstructions, have no basket immediately above it, and have a basket immediately below it if it is not the bottom level.
34. The receiving-position check takes a source-position argument but does not use it. It does not itself enforce profile length, destination compatibility, or basket type.
35. Mission conflict detection reserves an entire column/base position number across stored next and active missions, not just the individual level.
36. Rotating positions, exit conveyors, and destacker waiting/working positions are excluded from normal crane-position selection. They are represented on the display and controlled through the conveyor interface.

## 4. Operator interaction and displays

Sources: [mW.cpp:274](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/mW.cpp:274>), [allPos.cpp:389](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/allPos.cpp:389>), [allPos.cpp:411](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/allPos.cpp:411>), [pos.cpp:328](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:328>), [pos.cpp:483](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:483>), [gView.cpp:22](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/gView.cpp:22>).

37. Starts in Missions mode. Provides Missions, Exports, and Modify radio modes.
38. The left toolbox contains automatic controls and separate queues for HCB/SB, anodizing, packing destacker 1, and packing destacker 2.
39. Selecting the automatic-control toolbox page returns selection to Missions mode. On an export page, Exports mode makes layout clicks target that queue.
40. In Missions mode, clicking an eligible occupied position selects the source and highlights receiving candidates.
41. Clicking an empty destination attempts a manual mission immediately; there is no additional confirmation dialog in this path.
42. Clicking the selected source again or another occupied position cancels the selected source. That cancellation branch does not restore source-selection display mode; Escape does restore it when a source is still selected.
43. Escape in the graphics view cancels a pending source selection and restores source highlighting.
44. Shift+left-click reloads a position from the database.
45. Ctrl+left-click or Ctrl+right-click opens the position-data dialog where the interactive item is available.
46. Modify mode changes eligible-position highlighting; an ordinary click has no dedicated Modify branch. Editing is reached through the Ctrl-click path.
47. The main position display shows basket numbers, inferred EPICS destination, export markers (`B`, `A`, `P1`, `P2`), profiles, lock symbols, and source/destination mission markers.
48. Hover details include position description, numeric position, coordinates, basket, inferred destination, and profile/order information.
49. Displays profile, quantity, length in mm, temper, sales order, order line, finish code, standard practice, customer shipping information, and next department in position tooltips.
50. The basket-details table additionally exposes stop time and per-profile destination; it is read-only.
51. Shows active and next mission endpoints, PLC step text, crane coordinates/targets, readiness icon, and recent messages beside the layout.
52. Mouse wheel zooms about the cursor; middle-button dragging pans. Non-Debug zoom scaling is constrained between approximately 0.01 and 1.0.
53. Zoom extents fits a predefined plant rectangle, rather than dynamically deriving the entire scene extent.
54. F1 in the main window or full HMI writes stop command 226 and then 0 to `toPcI[22]`.
55. Ctrl+Q opens Qt About information; it is not a quit shortcut here.
56. The PLC communications action opens the communication table dialog after `getPassword()` succeeds. Its tooltip advertises Ctrl+P, but the shown constructor does not assign that keyboard shortcut.
57. Toolbar actions open conveyors, destacker, and Basic HMI dialogs and request basket return to crane 1.
58. The “24 feet” toolbar control sends command 6 to `PcCom[9]`, returns it to 0, and uses `toPcI[23].29` feedback to alternate its label/state with “more than 24 feet.” Interpretation is delegated to the PLC.

## 5. Manual mission validation and lifecycle

Sources: [carriage.cpp:159](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:159>), [carriage.cpp:180](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:180>), [carriage.cpp:289](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:289>), [carriage.cpp:457](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:457>), [carriage.cpp:490](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:490>), [carriage.cpp:530](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:530>).

59. The manual click path requires PLC idle state and readiness for a new mission.
60. Readiness requires next-from, next-to, and next-status values to be zero/idle, no pending in-memory next endpoints, and no basket on the carriage.
61. Mission validation rejects a communication error, missing endpoint, empty source, occupied destination, source not ready to send, or destination not ready to receive.
62. Rejects locked source/destination positions, inaccessible or unsupported stacks, and any source/destination column already referenced by a database mission.
63. Attempts to reject duplicate next/active missions. Its index comparisons contain assignment operators; see section 14.
64. On acceptance, sets in-memory next endpoints, persists them to the `next` row, and records messages.
65. Writes source ID to `toPcI[0]`, destination ID to `[1]`, and start status 1 to `[2]`.
66. When PLC status becomes 1, promotes next endpoints into the active database row, clears next endpoints, and acknowledges 1 in `toPcI[12]` if promotion succeeds.
67. When PLC status becomes 2 (“basket on carriage”), moves the database basket from source to carriage, provided the source has a basket, carriage is empty, and destination is still empty; then acknowledges 2.
68. When PLC status becomes 3 (“finished”), moves the basket from carriage to destination, provided the carriage has a basket and destination is empty.
69. Successful completion updates export queues, emits an export-list change, checks return-conveyor dispatch for column 700, clears stored mission endpoints, and acknowledges 3.
70. If completion finds an occupied destination or empty carriage with an active mission still present, logs an error rather than performing the expected transfer.
71. PLC idle state with active PC endpoints is treated as a mismatch and repeatedly logged by the one-second check; it does not automatically reconcile those endpoints.
72. Status 4 invokes abort recovery, attempting to return a basket from destination or carriage to source. The current success/clear condition is anomalous; see section 14.
73. Unrecognized mission status values have no branch in the dispatcher. Status 10 is declared but is not processed there.
74. `clearNextMission()` clears in-memory/database next endpoints and zeroes PLC `[0]`, `[1]`, and `[2]`.
75. `clearMissionInDb()` clears both stored next and active rows, clears active in-memory endpoints, and writes finished status 3 to the PC acknowledgement. It does not itself move a basket, zero PLC active endpoints, or clear the in-memory next pointers.
76. On startup, a complete stored active mission produces a warning and asks whether to clear it. Choosing Yes clears it and tells the operator basket numbers need correction; choosing No restores active endpoints.
77. Startup also tries to submit a complete stored next mission using normal `missionStart()` validation. Existing database reservations can reject that attempt; restoration is not guaranteed.
78. Basket transfers set the destination record before clearing the source and write position history. There is no explicit transaction surrounding these steps in the application code.
79. An invalid transfer with empty source or occupied destination calls the fatal exit/error helper. Certain null-endpoint logging paths also dereference the null pointer before reporting it.

Mission protocol:

| Field | PLC address |
|---|---|
| Next source / destination / status | `toPcI[0]` / `[1]` / `[2]` |
| Active source / destination | `toPcI[10]` / `[11]` |
| PC acknowledgement / PLC status | `toPcI[12]` / `[13]` |
| Crane step | `toPcI[18]` |

Step display: 0 idle; 1 active mission; 2 XY load target; 3 Z load target; 4 pins get basket; 5 Z zero; 6 XY unload target; 7 Z unload target; 8 pins release basket; 9 Z zero; 10 finished. These are PLC-reported steps; the PC does not implement the motor sequence.

## 6. Automatic mission selection

Sources: [autoWdg.cpp:3](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/autoWdg.cpp:3>), [crane.cpp:41](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/crane.cpp:41>), [crane.cpp:122](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/crane.cpp:122>), [allPos.cpp:262](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/allPos.cpp:262>), [allPos.cpp:297](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/allPos.cpp:297>).

80. Has seven independent checkboxes: HCB/SB export, packing destacker 1 export, packing destacker 2 export, anodizing export, north import, south import, and EPICS queue population.
81. Automatic options start unchecked with `autoMode = 0`. No restoration/persistence of those choices is present in the constructor.
82. The scheduler returns immediately while the PLC carriage status is not idle. When idle it refreshes export markers even if no automatic option is enabled.
83. Selects only the first basket by ascending priority from each enabled export queue. It does not scan lower-priority baskets if the head cannot move.
84. Automatic source search permits columns 670, 690, 1040, 1050, 1060, 700, 1080, and 1090, requiring an unlocked matching position.
85. Candidate validation also requires accessibility and absence of a stored mission reservation.
86. HCB/SB export requires no basket at to-rotating-north, rotating-north, or destacker waiting. The working destacker itself is not in that availability expression.
87. Anodizing and packing candidates require no incoming-direction basket on to-rotating-north, rotating-north, exit 1, or exit 2. Outgoing destination values 1, 9, 11, and 12 are excluded from the definition of “incoming.” Actual destination occupancy is still checked by mission validation.
88. Each packing line is suppressed when its crane-3 queue count plus occupied working-position count is greater than 2. A count of exactly 2 is still allowed by this expression.
89. North import requires a basket at to-rotating-north with destination 6 (“To Press”), no reservation on that column, and an available return-stack level.
90. South import requires a basket at to-rotating-south with destination 7 (“In from Destacker”), no reservation on that column, and an available return-stack level.
91. Return-stack selection fills 700.1 first, then .2 only with .1 occupied, then .3 only with .1 and .2 occupied. Inconsistent gaps do not yield a return position.
92. Mission priority is fixed: **HCB/SB export → anodizing export → packing 1 export → packing 2 export → north import → south import**.
93. All four export types send the crane to 1110.1/to-rotating-north. Their later route is encoded separately in conveyor destination data.
94. Imports target the selected level of column 700.
95. Once an earlier candidate branch is chosen, the scheduler does not try the next branch if `missionStart()` fails on additional validation. It can retry the same earlier candidate on later ticks.
96. Only when no direct candidate branch is selected does it try uncovering the head basket of each export queue, in HCB/SB, anodizing, packing 1, packing 2 order.
97. Uncovering recursively gathers baskets above the target and configured obstacle positions, chooses the last blocker, and tries to move it to a buffer.
98. For an oven blocker, buffer search order is 1080.1, 1090.1, 1080.2, 1090.2, 1080.3, 1090.3. A blocker already in one buffer can move only to the other buffer, lowest available level first.
99. Buffer targets must pass stack/occupancy checks and have no export marker; full mission validation subsequently applies as well.
100. Uncovering requires a nonzero automatic mode and the relevant broad route availability, but does not recheck the individual export checkbox or packing downstream count. Thus an unrelated enabled mode can permit relocation for a disabled export category.
101. With EPICS mode enabled, runs queue population after direct mission selection. If a successful uncover operation returns early, that invocation skips EPICS population.
102. Adding a mode uses bitwise OR; removing a mode uses XOR, which toggles the bit rather than unconditionally clearing it. Ordinary checkbox transitions normally pair enable/disable calls, but repeated removal calls would re-enable the bit.

## 7. EPICS contents and destination rules

Sources: [Gs.cpp:145](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/Gs.cpp:145>), [Gs.cpp:213](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/Gs.cpp:213>), [Gs.h:166](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/Gs.h:166>), [pos.cpp:308](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:308>), [pos.cpp:636](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:636>), [crane.cpp:213](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/crane.cpp:213>).

103. Joins `RackDetail` to `SOItem` by sales order and line, adding latest matching EXTRUDE stop time from `ShiftProd` within the date window beginning seven days before today's midnight.
104. Loads only racks numbered 1 through 130 with nonzero piece count. Basket numbers outside that range may exist in positions but receive no contents from this query.
105. Converts order length to millimetres using `OrdLen * 1000`, then stores it as an integer in the contents structure.
106. Sorts results by stop time descending. The position's “top contents” method uses the last row; its “bottom contents” method uses the first. Therefore the code calls the older/null-end of this ordering “top”; physical stack order is not independently verified.
107. A profile whose `NextDeptNum` does not contain `DESTACK` is classified as None.
108. Otherwise, standard practice containing `ANODIZING` or `PAINT` is classified HCB.
109. Otherwise, a trimmed finish code other than `000` is classified HCB.
110. With finish code `000`, any customer text containing `STARLINE`, `STAARC`, or `MILTAC2` is classified HCB.
111. With finish code `000`, any standard-practice text containing `CUTBACK`, `STOCK`, `DEPOT`, `O TEMPER @ AGGRESSIVE`, `CRIMP`, `PUNCH`, `AZOBRADE`, `AZO`, `ANODIZE`, `ANO`, `PRETREAT`, or `COMPONENT MILL` is classified HCB.
112. Remaining DESTACK profiles are classified HCA. Text matching uses case-insensitive substring checks, not exact codes or word boundaries.
113. The active production classifier returns None, HCA, or HCB; it does **not** return anodizing/Out. An EPICS-to-anodizing queue branch exists but is not reached through this classifier in the normal build.
114. EPICS queue population considers the highest occupied level in each of columns 670, 690, 1040, 1050, 1060, south buffer, and north buffer, in that order. It does not inspect every buried basket or include return column 700 in this automatic population list.
115. Routes HCB candidates to the HCB/SB queue, and tries HCA candidates for packing line 1 then packing line 2.
116. HCB and packing auto-add helpers skip candidates already carrying an export marker.
117. HCB queue population counts working/waiting destacker baskets, correctly routed north rotating/handoff baskets, and the HCB queue. When count is zero it adds the candidate; when count is at most 2 it requires a matching sales order and line against the selected existing basket.
118. Packing population combines its crane-2 queue, crane-3 queue, and occupied destination working position in source/priority order. It similarly permits addition at count ≤2 and otherwise requires matching sales order/line when the pipeline is nonempty.
119. The matching helper named `getBottomProfileContentsDataOfBasket()` also returns the last row of descending stop-time results, unlike the position object's bottom accessor. This inconsistency affects which order/line is compared.
120. A single pass can add at a count of 2, producing 3 entries. These conditions are admission thresholds, not strict maximum pipeline sizes of 2.

## 8. Export queues and conveyor destination handoff

Sources: [allPos.cpp:411](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/allPos.cpp:411>), [allPos.cpp:469](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/allPos.cpp:469>), [allPos.cpp:497](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/allPos.cpp:497>), [expWdg.cpp:3](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/expWdg.cpp:3>), [expWdg.cpp:135](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/expWdg.cpp:135>), [pos.cpp:145](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:145>), [G.h:114](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/G.h:114>).

121. Stores four independent ordered export queues with unique basket and unique priority columns per queue table.
122. In Exports mode, clicking a basket removes it from all four queues, then appends it to the selected category. Packing 1/2 clicks first require maximum loaded profile length ≤7660 mm; longer baskets are silently left unchanged.
123. Appending uses the current maximum priority plus 1 and avoids a duplicate within the selected queue.
124. Queue widgets show basket numbers and queue counts, preserve selection where possible, and display “No Baskets to export” when empty.
125. Operators can move a selected basket up/down one priority position or remove it. Reordering swaps priorities through a temporary value of -1.
126. Queue rows are always marked changeable in this implementation; the widget does not protect a mission's selected basket from reorder/removal.
127. On arrival at north handoff, reads the basket's queue memberships and sets PLC destination codes according to the table below.
128. For a packing handoff with destination 12, also adds the basket to the corresponding crane-3 export queue, if absent.
129. If more than one membership exists, later `if` branches can override earlier destination decisions; packing 2 is evaluated last. A basket present in both packing tables can be inserted into both crane-3 tables.
130. After a successful crane delivery to north handoff, deletes that basket from all four crane-2 queues. Intermediate relocation to a buffer leaves the stored queue entry intact; the marker is cleared then restored on a later refresh.
131. A handoff basket absent from all four queues causes no destination write in this setter; it does not automatically clear stale conveyor destination data.
132. Destination writes append timestamp, basket, and destination to `tempLogSetDestinations.txt` in the working directory. Failure to open that file calls `exit(0)`.

| Queue / direction | Destination code | Code's length condition |
|---|---:|---|
| HCB / Destacker For SB | 9 | Maximum length <7821 mm |
| Anodizing | 1 | Maximum length <7821 mm |
| HCB or anodizing overlength | 11 | Maximum length ≥7821 mm |
| Packing destacker 1 or 2 | 12 | Maximum length ≤7660 mm |
| Packing overlength | 11 | Maximum length >7660 mm |
| None | 0 | Available in conveyor editor |
| To Press / north import | 6 | PLC direction value |
| In from Destacker / south import | 7 | PLC direction value |

**Caveat:** the handoff setter tests `getMaximumLength()` before reloading the newly assigned basket's contents. The threshold table describes its expressions, but those expressions can evaluate the destination object's previous contents. See section 14.

## 9. Returning baskets toward crane 1

Sources: [allPos.cpp:539](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/allPos.cpp:539>), [carriage.cpp:564](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:564>).

133. After a completed crane delivery to column 700, counts occupied return levels and requests return when all three are occupied.
134. The toolbar “Send baskets to crane 1” calls the same write directly, allowing an operator request before all three levels are occupied.
135. Writes the basket number at 700.1 to old-oven `N11[17]`. It does not send all three basket numbers in that function or itself clear their database locations.
136. Emits a return signal after writing; the shown connection from that signal to automatic controls is commented out.

## 10. Position correction and lock editing

Sources: [pos.cpp:505](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:505>), [posDlg.cpp:3](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/posDlg.cpp:3>), [posDlg.cpp:106](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/posDlg.cpp:106>), [Gb2.cpp:37](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/Gb2.cpp:37>), [conPos.cpp:137](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/conPos.cpp:137>), [conPosDlg.cpp:3](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/conPosDlg.cpp:3>).

137. The ordinary position editor lets an operator enter basket 0 through `INT_MAX`, inspect contents/history, and change the position lock.
138. On acceptance, writes lock state immediately. The caller only applies the basket-number change if that exact position and index are absent from stored missions.
139. The position editor is opened using `myExec(false)`, so this path does not request a password.
140. If a proposed nonzero basket is reported at another position, shows its location and disables OK. The shared lookup returns a conflict only when exactly one database location is found; multiple duplicate locations are not correctly reported by that helper.
141. Replacing one nonzero basket directly with a different nonzero basket through a user correction can call the exit/error helper; clearing it first is a different path.
142. The main layout refuses the ordinary editor for most PLC-controlled conveyor positions and directs the user to the conveyors program.
143. The dedicated conveyor editor allows basket number and destination changes and displays Auto, Manual, Ready, Ready-for-job, and With-basket feedback.
144. Accepting a conveyor edit writes basket and destination directly to their PLC tables. There is no password or active-mission reservation check in that accept path beyond the duplicate-location UI check.
145. Conveyor destination choices are None, Anodizing, Destacker For SB, To Press, In from Destacker, Anodizing Overlength, and Packing Destacker.

## 11. Full HMI, Basic HMI, and recovery controls

Sources: [hmiDlg.cpp:3](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/hmiDlg.cpp:3>), [hmiDlg.cpp:144](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/hmiDlg.cpp:144>), [hmiDlg.cpp:212](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/hmiDlg.cpp:212>), [hmiSett.cpp:3](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/hmiSett.cpp:3>), [hmiB.cpp:3](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/hmiB.cpp:3>).

146. Full HMI shows X, Y, Z position, target, velocity, frequency, current, proximity, and on-target state. Each axis has trend plots for frequency, position, velocity, and current.
147. Displays next/active mission PLC fields and load/unload XYZ target coordinates as read-only memory widgets.
148. Provides Auto, Manual, Active Stop, Reset Alarms, X/Y jog, Z up/down, and a “Reset up” command.
149. Provides safety-pin lock/unlock and basket-pin lock/unlock commands with separate feedback indicators.
150. Provides abort, continue, end-active-mission, clear-next-mission, and clear-active-database-mission actions.
151. Each opening of the full HMI disables Abort, End, and Clear Active Database buttons. Double-clicking the Missions tab toggles this advanced-control lock, with a working password check on unlock.
152. Continue and Clear Next are not part of that advanced-button lock.
153. The full HMI itself opens without a password; its surrounding password guard is commented out.
154. Shows PLC alarm, warning, position, and incoming/outgoing interlock indicators; provides an interlock-bypass command and feedback label.
155. Encoder settings show current XYZ positions and offer resets for each axis. The settings password expression calls the password dialog but does not enforce the result; see section 14.
156. Basic HMI provides Reset Alarms plus alarm/warning displays.

All crane commands below target `toPcI[22]`; the HMI widgets are configured with release value 0. The actual momentary-button handling is in the linked HMI library.

| Control | Command |
|---|---:|
| X forward / backward | 1 / 2 |
| Y forward / backward | 3 / 4 |
| Basket pins unlock / lock | 5 / 6 |
| Z down / up | 7 / 8 |
| Safety pins unlock / lock | 11 / 12 |
| Auto / Manual / Reset alarms | 21 / 22 / 23 |
| End / Abort / Continue active mission | 24 / 26 / 48 |
| Reset Z / Y / X encoders | 101 / 102 / 103 |
| “Reset up” in Z axis panel | 102 (same value as Reset Y) |
| Interlocks bypass control | 106 |
| Active Stop, including F1 | 226 |

## 12. Conveyor and destacker controls

Sources: [conW.cpp:3](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/conW.cpp:3>), [conAllPos.cpp:3](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/conAllPos.cpp:3>), [conAllPos.cpp:150](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/conAllPos.cpp:150>), [conBtn.cpp:155](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/conBtn.cpp:155>), [destW.cpp:3](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/destW.cpp:3>).

157. Conveyor HMI shows eight positions, basket presence, proximity switches, direct movement controls, and active PLC command/alarm text.
158. Conveyor Auto, Manual, and Reset Alarms send 1, 2, and 3 respectively to `PcCom[9]`, with status feedback from conveyor 1.
159. Individual conveyor controls jog forward/backward. Other arrows request paired conveyor moves, rotating-conveyor left/right, lifts, and side movement.
160. Local graphical buttons write their command on mouse press when enabled/in maintenance mode, and write 0 on release. Feedback bits change their appearance.
161. On `bConveyorState[14]`, the code reads manual state but immediately overrides it with `true`, then enables maintenance display for positions and buttons. Several group buttons are also made maintenance-visible during construction.
162. Proximity graphics are driven by configured PLC bits and show active/inactive states; hovering exposes their PLC address.
163. The conveyor timer runs every second but its body is commented out. The shown rotating-animation update logic is also inactive.
164. Destacker HMI displays eight pincers, one OK/not-OK indicator each, and one enable/disable control each.
165. Pincer controls send values 1–8 to `toPcI[50]`; health is read from bits 0–7 of `[51]`, and enable/disable feedback from bits 8–15.
166. “Spacers on belts” sends 9 to `toPcI[50]` and reads feedback bit 16 of `[51]`.
167. Destacker alarm text is driven by `toPcI[52]`.

Conveyor command map — all writes target `PcCom[9]`, then 0 on release:

| Control group | Commands as labelled |
|---|---|
| Conveyor 1 / Exit 2 | FWD 102; BWD 103 |
| Conveyor 2 / Exit 1 | FWD 202; BWD 203 |
| Conveyor 3 / Rotating north | FWD 303; BWD 302 |
| Conveyor 4 / Rotating south | FWD 403; BWD 402 |
| Conveyor 6 / To rotating north | FWD 603; BWD 602 |
| Conveyor 7 / To rotating south | FWD 703; BWD 702 |
| Conveyor 8 / Waiting | FWD 802; BWD 803; Up 826; Down 827 |
| Conveyor 9 / Working | FWD 902; BWD 903; Up 926; Down 927 |
| Conveyor rotation | Left 503; Right 502 |
| Conveyors 1+2 | North 136; South 137 |
| Conveyors 2+3 | North 236; South 237 |
| Conveyors 6+3 | West 637; East 636 |
| Conveyors 7+4 | West 737; East 736 |
| Conveyors 3+8 | North 336; South 337 |
| Conveyors 4+9 | North 936; South 937 |
| Side move | West 1003; East 1002 |

## 13. Logging, resources, and inactive paths

Sources: [Gb2.cpp:5](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/Gb2.cpp:5>), [carriageW.cpp:44](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriageW.cpp:44>), [craneWdg.cpp:59](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/craneWdg.cpp:59>), [gItem.cpp:136](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/gItem.cpp:136>), [carriage.cpp:329](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:329>), [settingsDlg.cpp:3](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/settingsDlg.cpp:3>).

168. General log writes also write the verbose log. GUI log writes include messenger/type/time and also write verbose history. Position changes maintain separate basket/location history.
169. Truncates general message/type and detail fields to configured limits of 75 and 200 characters; Debug can show truncation notices.
170. Crane/carriage GUI log widgets load 100 historical entries and expose a larger popup with up to 300 entries. Errors are red; other messages are blue.
171. Consecutive identical GUI message/details/type combinations update the last record's timestamp instead of inserting another GUI entry.
172. Both crane widgets use messenger `crane`; their independently loaded lists are not a separate per-component audit namespace.
173. Position/basket dialogs query history for the selected basket; the table under position data also filters by basket, not solely by position.
174. Loads plant and basket graphics from DXF resources. A missing/unreadable DXF invokes the exit/error helper. Supported rendering includes points, lines, arcs, circles, ellipses, and text, with curved shapes approximated by line segments.
175. Uses the referenced text resources for PLC bit labels. These describe PLC-reported conditions; they do not implement the alarm detection or motion interlocks in C++.
176. Password checks return true unconditionally in Debug. In non-Debug they use a utility password validator and compiled fallback passwords; there is no per-user login/role model in the traced application paths.
177. Emulation code, disabled by the checked-in switches, can create/clear/populate mock EPICS tables, advance mission status at 500 ms intervals, simulate selected conveyor transfers, and move old-oven back rows forward when front rows are empty. Its mock schema does not establish all dependencies of the current production contents query, so it should not be assumed runnable unchanged.
178. Optional mission-timing SQL is guarded by `logMissionsTimes`. It is separate from the always-present GUI/position logging.
179. The generic ODBC/IP `settingsDlgClass` is compiled but has no caller in the traced application. Its fields are not an active configuration workflow. Its auto-crane-IP row also adds the basket-IP widget again instead of the auto-IP widget.
180. Resource `steps.cpp` is a bundled reference file; the active step text is built in `carriageW.cpp`. Bundled `conAlarms`, `conCommands`, and `conveyorWarnings` are not selected by the traced conveyor-widget constructors.
181. Generic `moveClass` contains arrow-command behavior but the current conveyor constructor does not instantiate entries in its movement vector; the visible jog controls use `buttonClass`.
182. Merely linking shared warehouse/packet helpers does not make them active crane-2 features. The traced application does not expose those other products' workflows.

## 14. Behaviors that appear unintended or need verification

These findings distinguish executable expressions from apparent intent. They were not exercised against hardware, and fixes were not made.

| Finding | Code effect / limitation | Source |
|---|---|---|
| Settings password result discarded | `if (getPassword(...), tr(...), this)` is a comma expression ending in a non-null `this`; settings opens even after a failed/cancelled password prompt. | [hmiSett.cpp:43](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/hmiSett.cpp:43>) |
| Clear-active password result discarded | Same comma-expression issue in the clear-active slot. The separate advanced-buttons unlock check still gates normal access to this button. | [hmiDlg.cpp:255](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/hmiDlg.cpp:255>) |
| Conveyor readiness table mismatch | Tables are registered/dispatched as `bConveyorState`, but the position receiver and upstream filter test `BConveyorState`. The intended north-handoff bit-12 readiness update does not match those notifications, and readiness defaults to true. | [pos.cpp:212](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:212>), [allPos.cpp:350](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/allPos.cpp:350>), [pos.h:47](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.h:47>) |
| Handoff length uses old contents | `setBasketId()` writes the new basket and evaluates `getMaximumLength()` before `loadFromDatabase()`. Routing and crane-3 admission can use an empty/stale destination contents vector. | [pos.cpp:145](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:145>) |
| Packing-2 enum mapping typo | `getExportCrane2EnumFromExportTableG()` checks the packing-1 table twice, so packing-2 maps to None in this helper. Later bulk refresh does map packing-2 correctly, but immediate assignment and same-pass EPICS decisions can be wrong. | [Gb2.cpp:131](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/Gb2.cpp:131>) |
| Abort branch reversed-looking condition | After a successful return move, `movedBasket` is nonzero, so it logs an error instead of clearing/acknowledging abort in that invocation. A subsequent one-second retry can then clear it if nothing remains to move. Returning a basket from destination to an already occupied source can invoke fatal transfer handling. | [carriage.cpp:589](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:589>) |
| Restarted next mission can reject itself | Stored next endpoints are retried through validation, which rejects columns already found in `c2missions`, including that same stored next row. | [carriage.cpp:159](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:159>), [carriage.cpp:215](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:215>) |
| Index assignment in duplicate comparisons | Expressions use `nextFromPosIndex = ...` and equivalent assignments instead of equality. These test assigned nonzero values, not matching stored indices. Earlier column reservation checks often reject first. | [carriage.cpp:215](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:215>) |
| Uncovering ignores individual enable switches | Any nonzero automatic mode can reach uncovering for all four stored export queues, subject to route checks. Downstream packing cap is not rechecked there. | [crane.cpp:122](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/crane.cpp:122>) |
| Earlier blocked candidate prevents fallback | Scheduler chooses an `if/else if` branch before the final mission validation and does not try lower-priority candidates when the chosen start fails. | [crane.cpp:122](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/crane.cpp:122>) |
| Maintenance mode forced on | The PLC-derived value is overwritten with `m=true`; PC-side visibility does not reliably reflect actual manual/maintenance state. Actual execution still depends on PLC logic. | [conAllPos.cpp:150](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/conAllPos.cpp:150>) |
| Z drawing/tooltip mapping mismatch | `crane.cpp` uses float index 1 as Z; HMI and crane widget use index 12 as Z and index 1 as X target. | [crane.cpp:91](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/crane.cpp:91>), [craneWdg.cpp:139](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/craneWdg.cpp:139>) |
| Profile ordering inconsistency | Position top is last descending-time row, position bottom first, but the global “bottom” lookup also returns last. Product continuity logic may compare the wrong physical layer. | [pos.cpp:636](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:636>), [crane.cpp:213](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/crane.cpp:213>) |
| Duplicate detection misses multiple locations | Conflict helper only returns a result for exactly one database match. More than one matching position yields no conflict text. | [Gb2.cpp:37](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/Gb2.cpp:37>) |
| Basket transfer is not atomic at this layer | Destination/source updates, mission promotion, queue swapping, and handoff enqueue/dequeue are multiple SQL calls without an explicit transaction here. Failure or another process can observe intermediate state. | [allPos.cpp:330](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/allPos.cpp:330>), [carriage.cpp:114](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:114>), [expWdg.cpp:135](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/expWdg.cpp:135>) |
| Assumes required records exist | Several queries index row 0 without checking count, restored endpoint pointers are dereferenced without validation, and a locked/missing return-bottom position can cause a null dereference in manual dispatch. | [carriage.cpp:159](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:159>), [carriage.cpp:215](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:215>), [allPos.cpp:548](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/allPos.cpp:548>) |
| Missing state initialization | `plcDestinationEnum` is not initialized in the shown position constructor/header; `isLocked` is only populated when exactly one database row is loaded. Scheduling/display before expected data arrives can therefore be undefined. | [pos.cpp:5](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:5>), [pos.cpp:308](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:308>), [pos.h:47](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.h:47>) |
| Queue recovery clears only some in-memory state | Clearing mission rows does not clear next pointers, unlike `clearNextMission()`. Partial recovery can leave PC state inconsistent with the cleared database. | [carriage.cpp:530](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/carriage.cpp:530>) |
| No active production EPICS anodizing result | Anodizing queue population is present, but the current classifier returns no anodizing value. Manual exports remain available. | [Gs.cpp:213](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/Gs.cpp:213>), [crane.cpp:275](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/crane.cpp:275>) |
| Local route log failure exits successfully | Cannot open `tempLogSetDestinations.txt` → `exit(0)`, potentially after database/PLC writes but before the remaining mission completion work. | [pos.cpp:134](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/pos.cpp:134>) |
| Controls with potentially misleading labels | “Reset up” shares command 102 with Reset Y; Ctrl+P is only tooltip text; ordinary Modify click has no editor action; old alarm position labels include 704 although only three return levels are instantiated. | [hmiDlg.cpp:200](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/hmiDlg.cpp:200>), [mW.cpp:274](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/mW.cpp:274>), [allPos.cpp:411](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/allPos.cpp:411>) |

## 15. Coverage and limits

The inventory covers application-level startup, configuration branches, position/contents state, manual and automatic scheduling, queue management, EPICS classification, PLC handshake, recovery, HMI commands, conveyor/destacker controls, data correction, logs, and graphics interaction visible in these sources.

It does not establish actual PLC acceptance of a command, movement timing, hardware safety behavior, library retry/write ordering, live schema/triggers, deployed source/resource parity, or runtime correctness. Resource messages are evidence of what the UI can label, not proof that C++ detects or enforces those conditions. There was no build or runtime test; this task changed only analysis documents.

## 16. Referenced PLC label catalog

The following appendix is transcribed from the resource text files passed to the active widget constructors. It preserves blank/reserved labels and original wording so every configured bit label can be reviewed. The table name comes from the constructor; each resource row supplies index, bit, and label. Linked widget-library parsing and any external-file override behavior have not been verified.


### plcAlarms.txt

Source: [plcAlarms.txt](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/resources/plcAlarms.txt>).

| PLC bit | Configured label |
|---|---|
| `toPcI[42].0` | Alarm 0.0:Stop cycle button pressed on OP1. |
| `toPcI[42].1` | Alarm 0.1:24 Volt DC alarm.Thermal overload CB1.Q29 tripped.PS alarm.//Relay K29-K34 off or aux selection switch is off.Input 424 off. |
| `toPcI[42].2` | Alarm 0.2:PLC restart.PLC outputs off. |
| `toPcI[42].3` | Alarm 0.3:Thermal overload CB1.Q1.Alarm from inverter U1.Inverter fault or ethernet communication problem. |
| `toPcI[42].4` | Alarm 0.4:Thermal overload CB1.Q2.Alarm from inverter U2.Inverter fault or ethernet communication problem. |
| `toPcI[42].5` | Alarm 0.5: |
| `toPcI[42].6` | Alarm 0.6: |
| `toPcI[42].7` | Alarm 0.7: |
| `toPcI[42].8` | Alarm 0.8: |
| `toPcI[42].9` | Alarm 0.9:Crane vertical carriage overspeed. |
| `toPcI[42].10` | Alarm 0.10:Crane carriage slips down. |
| `toPcI[42].11` | Alarm 0.11:Basket lock/unlock but no feedback from magnetic switch.Check valve.Check magnetic switch. |
| `toPcI[42].12` | Alarm 0.12:Safety pin lock/unlock but no feedback from magnetic switch.Check valve.Check magnetic switch. |
| `toPcI[42].13` | Alarm 0.13:Stop cycle button pressed on OP2. |
| `toPcI[42].14` | Alarm 0.14: |
| `toPcI[42].15` | Alarm 0.15: |
| `toPcI[42].16` | Alarm 0.16: |
| `toPcI[42].17` | Alarm 0.17:Crane carriage X axis overspeed. |
| `toPcI[42].18` | Alarm 0.18:Crane carriage X axis underspeed. |
| `toPcI[42].19` | Alarm 0.19:Crane carriage Z axis overspeed. |
| `toPcI[42].20` | Alarm 0.20:Crane carriage Z axis underspeed. |
| `toPcI[42].21` | Alarm 0.21:Crane carriage Y axis overspeed. |
| `toPcI[42].22` | Alarm 0.22:Crane carriage Y axis underspeed. |
| `toPcI[42].23` | Alarm 0.23: |
| `toPcI[42].24` | Alarm 0.24: |
| `toPcI[42].25` | Alarm 0.25:Step overtime for mission. |
| `toPcI[42].26` | Alarm 0.26: |
| `toPcI[42].27` | Alarm 0.27: |
| `toPcI[42].28` | Alarm 0.28: |
| `toPcI[42].29` | Alarm 0.29: |
| `toPcI[42].30` | Alarm 0.30:Alarm from thermal overload Q1,Q100,Q101,Q2. |
| `toPcI[42].31` | Alarm 31: |
| `toPcI[43].0` | Alarm 1.0: |
| `toPcI[43].1` | Alarm 1.1:Alarm from encoder X axis 20.20.20.71.Encoder fault or ethernet communication problem. |
| `toPcI[43].2` | Alarm 1.2:Alarm from encoder Y axis 20.20.20.72.Encoder fault or ethernet communication problem. |
| `toPcI[43].3` | Alarm 1.3:Alarm from encoder Z axis 20.20.20.73.Encoder fault or ethernet communication problem. |
| `toPcI[43].4` | Alarm 1.4: |
| `toPcI[43].5` | Alarm 1.5: |
| `toPcI[43].6` | Alarm 1.6: |
| `toPcI[43].7` | Alarm 1.7:Alarm from actual encoder values.Targets out of limit. |
| `toPcI[43].8` | Alarm 1.8:Alarm from actual encoder values.Actual encoder value out of limit. |
| `toPcI[43].9` | Alarm 1.9:Εthernet communication problem. |
| `toPcI[43].10` | Alarm 1.10: |
| `toPcI[43].11` | Alarm 1.11: |
| `toPcI[43].12` | Alarm 1.12: |
| `toPcI[43].13` | Alarm 1.13: |
| `toPcI[43].14` | Alarm 1.14:Alarm from horizontal/vertical motion stop. |
| `toPcI[43].15` | Alarm 1.15: |
| `toPcI[43].16` | Alarm 1.16: |
| `toPcI[43].17` | Alarm 1.17. |
| `toPcI[43].18` | Alarm 1.18: |
| `toPcI[43].19` | Alarm 1.19: |
| `toPcI[43].20` | Alarm 1.20: |
| `toPcI[43].21` | Alarm 1.21: |
| `toPcI[43].22` | Alarm 1.22: |
| `toPcI[43].23` | Alarm 1.23: |
| `toPcI[43].24` | Alarm 1.24: |
| `toPcI[43].25` | Alarm 1.25: |
| `toPcI[43].26` | Alarm 1.26: |
| `toPcI[43].27` | Alarm 1.27: |
| `toPcI[43].28` | Alarm 1.28: |
| `toPcI[43].29` | Alarm 1.29: |
| `toPcI[43].30` | Alarm 1.30: |
| `toPcI[43].31` | Alarm 1.31: |
| `toPcI[44].0` | Alarm 2.0: |
| `toPcI[44].1` | Alarm 2.1: |
| `toPcI[44].2` | Alarm 2.2: |
| `toPcI[44].3` | Alarm 2.3: |
| `toPcI[44].4` | Alarm 2.4: |
| `toPcI[44].5` | Alarm 2.5: |
| `toPcI[44].6` | Alarm 2.6: |
| `toPcI[44].7` | Alarm 2.7: |
| `toPcI[44].8` | Alarm 2.8: |
| `toPcI[44].9` | Alarm 2.9: |
| `toPcI[44].10` | Alarm 2.10: |
| `toPcI[44].11` | Alarm 2.11: |
| `toPcI[44].12` | Alarm 2.12: |
| `toPcI[44].13` | Alarm 2.13: |
| `toPcI[44].14` | Alarm 2.14: |
| `toPcI[44].15` | Alarm 2.15: |
| `toPcI[44].16` | Alarm 2.16: |
| `toPcI[44].17` | Alarm 2.17: |
| `toPcI[44].18` | Alarm 2.18: |
| `toPcI[44].19` | Alarm 2.19: |
| `toPcI[44].20` | Alarm 2.20: |
| `toPcI[44].21` | Alarm 2.21: |
| `toPcI[44].22` | Alarm 2.22: |
| `toPcI[44].23` | Alarm 2.23: |
| `toPcI[44].24` | Alarm 2.24: |
| `toPcI[44].25` | Alarm 2.25: |
| `toPcI[44].26` | Alarm 2.26: |
| `toPcI[44].27` | Alarm 2.27: |
| `toPcI[44].28` | Alarm 2.28: |
| `toPcI[44].29` | Alarm 2.29: |
| `toPcI[44].30` | Alarm 2.30: |
| `toPcI[44].31` | Alarm 2.31: |


### plcWarnings.txt

Source: [plcWarnings.txt](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/resources/plcWarnings.txt>).

| PLC bit | Configured label |
|---|---|
| `toPcI[46].0` | Warning 0.0:Crane can not move X,Y axis fwd-bwd.Axis Z proximity switch is not active.Crane must be up. |
| `toPcI[46].1` | Warning 0.1:Crane can not move X,Y axis fwd-bwd.SAfety pin is not locked.Safety pin must be locked. |
| `toPcI[46].2` | Warning 0.2: |
| `toPcI[46].3` | Warning 0.3:Crane can not move Z axis.Safety pin is not unlocked.Safety pin must be unlocked. |
| `toPcI[46].4` | Warning 0.4:Crane can not move Z axis.Basket pin is in position to release basket.Basket pin must be in position to release basket. |
| `toPcI[46].5` | Warning 0.5:Crane can not move Z axis.Basket pin is in position to get basket.Basket pin must be in position to get basket. |
| `toPcI[46].6` | Warning 0.6: |
| `toPcI[46].7` | Warning 0.7: |
| `toPcI[46].8` | Warning 0.8:Stop from stop cycle / alarms. |
| `toPcI[46].9` | Warning 0.9:Crane not in load position X,Y axis. |
| `toPcI[46].10` | Warning 0.10:Crane not in unload position X,Y axis. |
| `toPcI[46].11` | Warning 0.11: |
| `toPcI[46].12` | Warning 0.12: |
| `toPcI[46].13` | Warning 0.13: |
| `toPcI[46].14` | Warning 0.14: |
| `toPcI[46].15` | Warning 0.15:Crane carriage can not move vertical.Proximity switch vertical reset is 1. |
| `toPcI[46].16` | Warning 0.16: |
| `toPcI[46].17` | Warning 0.17: |
| `toPcI[46].18` | Warning 0.18: |
| `toPcI[46].19` | Warning 0.19: |
| `toPcI[46].20` | Warning 0.20: |
| `toPcI[46].21` | Warning 0.21: |
| `toPcI[46].22` | Warning 0.22: |
| `toPcI[46].23` | Warning 0.23: |
| `toPcI[46].24` | Warning 0.24: |
| `toPcI[46].25` | Warning 0.25: |
| `toPcI[46].26` | Warning 0.26: |
| `toPcI[46].27` | Warning 0.27: |
| `toPcI[46].28` | Warning 0.28:Interlock signal from loading position is not active. |
| `toPcI[46].29` | Warning 0.29:Interlock signal from unloading position is not active. |
| `toPcI[46].30` | Warning 0.30:Ethernet communication error.Read message to conveyors PLC error. |
| `toPcI[46].31` | Warning 0.31:Ethernet communication error.Write message to conveyors PLC error. |


### plcPositions.txt

Source: [plcPositions.txt](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/resources/plcPositions.txt>).

| PLC bit | Configured label |
|---|---|
| `toPcI[40].0` | Position X,Y 661-664. |
| `toPcI[40].1` | Position X,Y 681-684. |
| `toPcI[40].2` | Position X,Y 671-674. |
| `toPcI[40].3` | Position X,Y 691-694. |
| `toPcI[40].4` | Position X,Y 701-704. |
| `toPcI[40].5` | Position X,Y 1041-1044. |
| `toPcI[40].6` | Position X,Y 1051-1054. |
| `toPcI[40].7` | Position X,Y 1061-1064. |
| `toPcI[40].8` | Position X,Y 1111. |
| `toPcI[40].9` | Position X,Y 1101. |
| `toPcI[40].10` | (blank) |
| `toPcI[40].11` | (blank) |
| `toPcI[40].12` | (blank) |
| `toPcI[40].13` | (blank) |
| `toPcI[40].14` | (blank) |
| `toPcI[40].15` | (blank) |
| `toPcI[40].16` | (blank) |
| `toPcI[40].17` | (blank) |
| `toPcI[40].18` | (blank) |
| `toPcI[40].19` | (blank) |
| `toPcI[40].20` | (blank) |
| `toPcI[40].21` | (blank) |
| `toPcI[40].22` | (blank) |
| `toPcI[40].23` | (blank) |
| `toPcI[40].24` | (blank) |
| `toPcI[40].25` | (blank) |
| `toPcI[40].26` | (blank) |
| `toPcI[40].27` | (blank) |
| `toPcI[40].28` | (blank) |
| `toPcI[40].29` | (blank) |
| `toPcI[40].30` | (blank) |
| `toPcI[40].31` | (blank) |


### plcInterlocksFrom.txt

Source: [plcInterlocksFrom.txt](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/resources/plcInterlocksFrom.txt>).

| PLC bit | Configured label |
|---|---|
| `toPcI[27].0` | Signal from oven1:Not safe from position 661-664. |
| `toPcI[27].1` | Signal from oven1:Not safe from position 671-674. |
| `toPcI[27].2` | Signal from oven1:Not safe from position 681-684. |
| `toPcI[27].3` | Signal from oven1:Not safe from position 691-694. |
| `toPcI[27].4` | Signal from oven2:Not safe from position 1041-1044. |
| `toPcI[27].5` | Signal from oven2:Not safe from position 1051-1054. |
| `toPcI[27].6` | Signal from oven2:Not safe from position 1061-1064. |
| `toPcI[27].7` | Signal from conveyor 6:Not safe from position 1111. |
| `toPcI[27].8` | Signal from return conveyor1:Not safe from position 701-703. |
| `toPcI[27].9` | Signal from return conveyor1:Position 701 photocell "on". |
| `toPcI[27].10` | Signal from return conveyor1:Position 702-3 photocell "off". |
| `toPcI[27].11` | (blank) |
| `toPcI[27].12` | (blank) |
| `toPcI[27].13` | (blank) |
| `toPcI[27].14` | (blank) |
| `toPcI[27].15` | (blank) |
| `toPcI[27].16` | (blank) |
| `toPcI[27].17` | (blank) |
| `toPcI[27].18` | (blank) |
| `toPcI[27].19` | (blank) |
| `toPcI[27].20` | (blank) |
| `toPcI[27].21` | (blank) |
| `toPcI[27].22` | (blank) |
| `toPcI[27].23` | (blank) |
| `toPcI[27].24` | (blank) |
| `toPcI[27].25` | (blank) |
| `toPcI[27].26` | (blank) |
| `toPcI[27].27` | (blank) |
| `toPcI[27].28` | (blank) |
| `toPcI[27].29` | (blank) |
| `toPcI[27].30` | (blank) |
| `toPcI[27].31` | (blank) |


### plcInterlocksTo.txt

Source: [plcInterlocksTo.txt](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/resources/plcInterlocksTo.txt>).

| PLC bit | Configured label |
|---|---|
| `toPcI[28].0` | Signal to oven 1:Position 661-664 locked. |
| `toPcI[28].1` | Signal to oven 1:Position 671-674 locked. |
| `toPcI[28].2` | Signal to oven 1:Position 681-684 locked. |
| `toPcI[28].3` | Signal to oven 1:Position 691-694 locked. |
| `toPcI[28].4` | Signal to oven 2:Position 1041-1044 locked. |
| `toPcI[28].5` | Signal to oven 2:Position 1051-1054 locked. |
| `toPcI[28].6` | Signal to oven 2:Position 1061-1064 locked. |
| `toPcI[28].7` | Signal to return conveyor1:Position 701-702 locked. |
| `toPcI[28].8` | Signal to conveyor 6:Position 1111 locked. |
| `toPcI[28].9` | Signal to conveyor 7:Position 1101 locked. |
| `toPcI[28].10` | (blank) |
| `toPcI[28].11` | (blank) |
| `toPcI[28].12` | (blank) |
| `toPcI[28].13` | (blank) |
| `toPcI[28].14` | (blank) |
| `toPcI[28].15` | (blank) |
| `toPcI[28].16` | (blank) |
| `toPcI[28].17` | (blank) |
| `toPcI[28].18` | (blank) |
| `toPcI[28].19` | (blank) |
| `toPcI[28].20` | (blank) |
| `toPcI[28].21` | (blank) |
| `toPcI[28].22` | (blank) |
| `toPcI[28].23` | (blank) |
| `toPcI[28].24` | (blank) |
| `toPcI[28].25` | (blank) |
| `toPcI[28].26` | (blank) |
| `toPcI[28].27` | (blank) |
| `toPcI[28].28` | (blank) |
| `toPcI[28].29` | (blank) |
| `toPcI[28].30` | (blank) |
| `toPcI[28].31` | (blank) |


### conveyorCommands.txt

Source: [conveyorCommands.txt](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/resources/conveyorCommands.txt>).

| PLC bit | Configured label |
|---|---|
| `PcCom[0].0` | Command  0.0: |
| `PcCom[0].1` | Command  0.1:Command 1->2.Basket is moving from conveyor BC1 to conveyor BC2. |
| `PcCom[0].2` | Command  0.2:Command 2->3.Basket is moving from conveyor BC2 to conveyor BC3. |
| `PcCom[0].3` | Command  0.3:Command 3->8.Basket is moving from conveyor BC3 to conveyor BC8. |
| `PcCom[0].4` | Command  0.4: |
| `PcCom[0].5` | Command  0.5: |
| `PcCom[0].6` | Command  0.6:Command 6->3.Basket is moving from conveyor BC6 to conveyor BC3. |
| `PcCom[0].7` | Command  0.7: |
| `PcCom[0].8` | Command  0.8:Command 8->9.Basket is moving from conveyor BC8 to conveyor BC9. |
| `PcCom[0].9` | Command  0.9:Command 9->4.Basket is moving from conveyor BC9 to conveyor BC4. |
| `PcCom[0].10` | Command 0.10Command 4->7.Basket is moving from conveyor BC4 to conveyor BC7. |
| `PcCom[0].11` | Command 0.11: |
| `PcCom[0].12` | Command 0.12: |
| `PcCom[0].13` | Command 0.13:Command 2->1.Basket is moving from conveyor BC2 to conveyor BC1. |
| `PcCom[0].14` | Command 0.14:Command 3->6.Basket is moving from conveyor BC3 to conveyor BC6. |
| `PcCom[0].15` | Command 0.15: |
| `PcCom[0].16` | Command 0.16: |
| `PcCom[0].17` | Command 0.17:Command 3->2.Basket is moving from conveyor BC3 to conveyor BC2. |
| `PcCom[0].18` | Command 0.18: |
| `PcCom[0].19` | Command 0.19: |
| `PcCom[0].20` | Command 0.20: |
| `PcCom[0].21` | Command 0.21: |
| `PcCom[0].22` | Command 0.22: |
| `PcCom[0].23` | Command 0.23: |
| `PcCom[0].24` | Command 0.24: |
| `PcCom[0].25` | Command 0.25: |
| `PcCom[0].26` | Command 0.26:Rotation to BC2. |
| `PcCom[0].27` | Command 0.27:Rotation to BC6. |
| `PcCom[0].28` | Command 0.28: |
| `PcCom[0].29` | Command 0.29: |
| `PcCom[0].30` | Command 0.30: |
| `PcCom[0].31` | Command 0.31: |


### conveyorAlarms.txt

Source: [conveyorAlarms.txt](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/resources/conveyorAlarms.txt>).

| PLC bit | Configured label |
|---|---|
| `PcCom[8].0` | Alarm  0.0:CB1.24 Volt DC alarm. |
| `PcCom[8].1` | Alarm  0.1:CB1.Stop cycle button pressed on OP1. |
| `PcCom[8].2` | Alarm  0.2:CB1.Thermal overload Q28.Alarm from inverter U28.//Inverter fault or ethernet communication problem. |
| `PcCom[8].3` | Alarm  0.3:CB1. |
| `PcCom[8].4` | Alarm  0.4:CB1.Thermal overload Q280,281,282,283,284,285,17,18. |
| `PcCom[8].5` | Alarm  0.5:CB1. |
| `PcCom[8].6` | Alarm  0.6:CB1.Overtime for active command.Possible problem. |
| `PcCom[8].7` | Alarm  0.7:CB1. |
| `PcCom[8].8` | Alarm  0.8:CB1. |
| `PcCom[8].9` | Alarm  0.9: |
| `PcCom[8].10` | Alarm 0.10: |
| `PcCom[8].11` | Alarm 0.11: |
| `PcCom[8].12` | Alarm 0.12: |
| `PcCom[8].13` | Alarm 0.13: |
| `PcCom[8].14` | Alarm 0.14: |
| `PcCom[8].15` | Alarm 0.15: |
| `PcCom[8].16` | Alarm 0.16: |
| `PcCom[8].17` | Alarm 0.17: |
| `PcCom[8].18` | Alarm 0.18: |
| `PcCom[8].19` | Alarm 0.19: |
| `PcCom[8].20` | Alarm 0.20: |
| `PcCom[8].21` | Alarm 0.21: |
| `PcCom[8].22` | Alarm 0.22: |
| `PcCom[8].23` | Alarm 0.23: |
| `PcCom[8].24` | Alarm 0.24: |
| `PcCom[8].25` | Alarm 0.25: |
| `PcCom[8].26` | Alarm 0.26: |
| `PcCom[8].27` | Alarm 0.27: |
| `PcCom[8].28` | Alarm 0.28: |
| `PcCom[8].29` | Alarm 0.29: |
| `PcCom[8].30` | Alarm 0.30: |
| `PcCom[8].31` | Alarm 0.31: |


### destackerAlarms.txt

Source: [destackerAlarms.txt](<C:/Users/aflores/OneDrive - Lorval Capital Management/Documents/sourceCode/assimakis/development/exe/apex/basketCrane2/resources/destackerAlarms.txt>).

| PLC bit | Configured label |
|---|---|
| `toPcI[52].0` | Alarm 0.0:24 Volt DC alarm.Thermal overload CB1.Q29 tripped.PS alarm.//Relay K29-K34 off or aux selection switch is off.Input 424 off. |
| `toPcI[52].1` | Alarm 0.1:Stop cycle button pressed on Destacker2 OP. |
| `toPcI[52].2` | Alarm 0.2:Thermal overload CB1:Q5,Q500,Q501,Q6. |
| `toPcI[52].3` | Alarm 0.3:Thermal overload CB1:Q8,Q800. |
| `toPcI[52].4` | Alarm 0.4: |
| `toPcI[52].5` | Alarm 0.5:Thermal overload CB1.Q5.Alarm from inverter U5.Inverter fault or ethernet communication problem. |
| `toPcI[52].6` | Alarm 0.6:Εthernet module 1769-AENTR fault or communication problem. |
| `toPcI[52].7` | Alarm 0.7: |
| `toPcI[52].8` | Alarm 0.8: |
| `toPcI[52].9` | Alarm 0.9: |
| `toPcI[52].10` | Alarm 0.10:Alarm from encoder vertical axis.Encoder card fault.Reset vertical in manual mode. |
| `toPcI[52].11` | Alarm 0.11:Alarm from encoder horizontal axis.Encoder card fault.Reset horizontal in manual mode. |
| `toPcI[52].12` | Alarm 0.12: |
| `toPcI[52].13` | Alarm 0.13:Destacker2 valve "hooks inside" but no feedback from magnetic switch.Check valve.Check magnetic switch. |
| `toPcI[52].14` | Alarm 0.14:Destacker2 valve  "hooks out" but no feedback from magnetic switch.Check valve.Check magnetic switch. |
| `toPcI[52].15` | Alarm 0.15:Destacker2 horizontal axis overspeed. |
| `toPcI[52].16` | Alarm 0.16:Destacker2 horizontal axis underspeed. |
| `toPcI[52].17` | Alarm 0.17:Destacker2 vertical axis overspeed. |
| `toPcI[52].18` | Alarm 0.18:Destacker2 vertical axis underspeed. |
| `toPcI[52].19` | Alarm 0.19: |
| `toPcI[52].20` | Alarm 0.20: |
| `toPcI[52].21` | Alarm 0.21: |
| `toPcI[52].22` | Alarm 0.22: |
| `toPcI[52].23` | Alarm 0.23: |
| `toPcI[52].24` | Alarm 0.24: |
| `toPcI[52].25` | Alarm 0.25: |
| `toPcI[52].26` | Alarm 0.26: |
| `toPcI[52].27` | Alarm 0.27: |
| `toPcI[52].28` | Alarm 0.28: |
| `toPcI[52].29` | Alarm 0.29: |
| `toPcI[52].30` | Alarm 0.30: |
| `toPcI[52].31` | Alarm 0.31: |
