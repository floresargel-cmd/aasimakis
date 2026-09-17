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
