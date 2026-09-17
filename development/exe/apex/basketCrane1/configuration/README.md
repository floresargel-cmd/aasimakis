# BasketCrane.Configuration

This static-library project owns runtime configuration for the basket-crane
application.

## Responsibilities

- `ConfigurationManager` loads, expands, and validates the INI file once.
- `Settings` exposes immutable, typed configuration objects grouped by domain.
- `[EmailAlerts]` configures error notifications independently of crane operation.
  Invalid mail settings disable email with a log warning. See `../LOGGING.md` for
  the supported SMTP settings, batching interval and subject prefix.
- `DatabaseConfigurator` applies a typed database profile to `QSqlDatabase`.
- `[Environment] mode` selects `Test`, `Live`, or `LiveObserver`. LiveObserver
  reads Live profiles while centrally blocking SQL mutations and PLC output.
  Both Live modes require the current computer in `liveEnabledHosts`.

Application code consumes typed settings and does not read INI keys directly.
Protocol constants and physical model definitions remain in the application
because changing them is a software/protocol change rather than deployment
configuration.

## Bright pink basket restrictions
The header Settings button configures blocked destinations for baskets containing T1, T4, or F material. Checked locations are blocked. Save applies to new move requests immediately and persists in the active INI file under [BasketRestrictions], locations (pipe-separated). Cancel discards edits; Restore Defaults selects all nine locations (100, 110, 120, 130, 140, 150, 220, 280, 290), pending Save. An absent key uses these defaults; an explicitly empty value disables location restrictions. Already dispatched moves are unaffected. Basket tooltips show the saved list. Settings apply to this installation's active configuration file.

Settings separates the nine default restrictions from 28 additional crane-accessible locations. The default list is always visible. Check an additional location to restrict it; Restore Defaults clears additional selections. Internal oven/stacker/conveyor-only positions are excluded. The shared location catalog is defined by ConfigurationManager::AvailableBasketRestrictionLocations and must follow crane destination changes in ap.cpp.

Under [Application], showSettingsButton=true shows the Settings button; false hides it. The default is true when omitted. Restart the application after changing this value. Existing basket restrictions remain enforced when the button is hidden.
