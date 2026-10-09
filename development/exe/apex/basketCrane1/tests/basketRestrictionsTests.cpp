#include "../configuration/ConfigurationManager.h"
#include <QCoreApplication>
#include <QTemporaryDir>
#include <QFile>
#include <QSettings>
#include <cstdio>
#include <cstdlib>
using BasketCrane::Configuration::ConfigurationManager;
struct basketPositionClass {
    bool restricted = true;
    bool hasNonAgeableTemperMaterial() { return restricted; }
    bool canMoveToPosition(int destinationNumber);
};
#include "basketRestrictionMethod.inc"
static void check(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "FAIL: %s\n", message); std::exit(1); }
}
int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    check(argc == 2, "pass source ini path");
    QTemporaryDir temp;
    const QString path = temp.path() + "/test.ini";
    check(QFile::copy(QString::fromLocal8Bit(argv[1]), path), "copy fixture");
    { QSettings ini(path, QSettings::IniFormat); ini.remove("BasketRestrictions"); ini.sync(); }
    auto& config = ConfigurationManager::Instance();
    QString error;
    check(config.Load(path, &error), "load old config without restriction section");
    const QStringList defaults = QString("100,110,120,130,140,150,220,280,290").split(',');
    check(config.RestrictedBasketLocations().join("|") == defaults.join("|"), "all nine locations blocked by default");
    basketPositionClass basket;
    foreach (const QString& location, defaults)
        check(!basket.canMoveToPosition(location.toInt()), "pink basket blocked at each of nine locations");
    check(basket.canMoveToPosition(160), "unlisted destination remains allowed");
    basket.restricted = false;
    foreach (const QString& location, defaults)
        check(basket.canMoveToPosition(location.toInt()), "other baskets unaffected");
    basket.restricted = true;
    const QStringList custom = QStringList() << "140" << "150" << "200" << "500" << "880";
    check(config.SaveRestrictedBasketLocations(custom, &error), "save changed locations");
    check(config.RestrictedBasketLocations().join("|") == custom.join("|"), "apply immediately");
    check(basket.canMoveToPosition(100) && !basket.canMoveToPosition(140) && !basket.canMoveToPosition(150), "move gate uses saved selection immediately");
    check(config.Load(path, &error) && config.RestrictedBasketLocations().join("|") == custom.join("|"), "persist after reload");
    check(!config.SaveRestrictedBasketLocations(QStringList() << "bogus", &error), "reject invalid location");
    check(config.RestrictedBasketLocations().join("|") == custom.join("|"), "failed save keeps active restrictions");
    check(config.SaveRestrictedBasketLocations(QStringList(), &error), "save explicitly empty selection");
    check(config.Load(path, &error) && config.RestrictedBasketLocations().isEmpty(), "empty selection persists");
    const QStringList available = ConfigurationManager::AvailableBasketRestrictionLocations();
    check(available.size() == 37, "all 37 crane destinations available");
    check(config.SaveRestrictedBasketLocations(available, &error), "save all available locations");
    check(config.Load(path, &error), "reload expanded restriction list");
    foreach (const QString& location, available)
        check(!basket.canMoveToPosition(location.toInt()), "additional destinations enforced");
    check(!config.SaveRestrictedBasketLocations(QStringList() << "600", &error), "reject internal oven position");
    check(config.SaveRestrictedBasketLocations(ConfigurationManager::DefaultRestrictedBasketLocations(), &error), "restore defaults");
    check(!basket.canMoveToPosition(140) && basket.canMoveToPosition(200) && basket.canMoveToPosition(880), "restore clears extras and retains defaults");
    { QSettings ini(path, QSettings::IniFormat); ini.setValue("BasketRestrictions/locations", "140|oops"); ini.sync(); }
    check(!config.Load(path, &error), "reject malformed persisted locations");
    std::puts("PASS: defaults, save, reload, invalid values, empty selection, restore defaults");
}
