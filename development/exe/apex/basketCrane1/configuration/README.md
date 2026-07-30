# BasketCrane.Configuration

This static-library project owns runtime configuration for the basket-crane
application.

## Responsibilities

- `ConfigurationManager` loads, expands, and validates the INI file once.
- `Settings` exposes immutable, typed configuration objects grouped by domain.
- `DatabaseConfigurator` applies a typed database profile to `QSqlDatabase`.

Application code consumes typed settings and does not read INI keys directly.
Protocol constants and physical model definitions remain in the application
because changing them is a software/protocol change rather than deployment
configuration.
