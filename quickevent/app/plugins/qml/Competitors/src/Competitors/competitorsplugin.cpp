#include "competitorsplugin.h"
#include "../thispartwidget.h"
#include "competitordocument.h"
#include "../registrationswidget.h"

#include <qf/qmlwidgets/framework/mainwindow.h>
#include <qf/qmlwidgets/framework/dockwidget.h>
#include <qf/qmlwidgets/action.h>
#include <qf/qmlwidgets/menubar.h>

#include <qf/core/model/sqltablemodel.h>
#include <qf/core/log.h>

#include <QQmlEngine>

namespace qfw = qf::qmlwidgets;
namespace qff = qf::qmlwidgets::framework;
namespace qfm = qf::core::model;
namespace qfs = qf::core::sql;

namespace Competitors {

CompetitorsPlugin::CompetitorsPlugin(QObject *parent)
	: Super(parent)
{
	connect(this, &CompetitorsPlugin::installed, this, &CompetitorsPlugin::onInstalled, Qt::QueuedConnection);
}

CompetitorsPlugin::~CompetitorsPlugin()
{
	if(m_registrationsDockWidget)
		m_registrationsDockWidget->savePersistentSettingsRecursively();
}

QObject *CompetitorsPlugin::createCompetitorDocument(QObject *parent)
{
	CompetitorDocument *ret = new CompetitorDocument(parent);
	if(!parent) {
		qfWarning() << "Parent is NULL, created class will have QQmlEngine::JavaScriptOwnership.";
		qmlEngine()->setObjectOwnership(ret, QQmlEngine::JavaScriptOwnership);
	}
	return ret;
}

void CompetitorsPlugin::onInstalled()
{
	qff::MainWindow *fwk = qff::MainWindow::frameWork();
	m_partWidget = new ThisPartWidget();
	fwk->addPartWidget(m_partWidget, manifest()->featureId());
	{
		m_registrationsDockWidget = new qff::DockWidget(nullptr);
		m_registrationsDockWidget->setObjectName("registrationsDockWidget");
		m_registrationsDockWidget->setWindowTitle(tr("Registrations"));
		fwk->addDockWidget(Qt::RightDockWidgetArea, m_registrationsDockWidget);
		m_registrationsDockWidget->hide();
		connect(m_registrationsDockWidget, &qff::DockWidget::visibleChanged, this, &CompetitorsPlugin::onRegistrationsDockVisibleChanged);
	}
	{
		auto *a = m_registrationsDockWidget->toggleViewAction();
		//a->setCheckable(true);
		a->setShortcut(QKeySequence("ctrl+shift+R"));
		fwk->menuBar()->actionForPath("view")->addActionInto(a);
	}
	emit nativeInstalled();
}

void CompetitorsPlugin::onRegistrationsDockVisibleChanged(bool on)
{
	if(on && !m_registrationsDockWidget->widget()) {
		auto *rw = new RegistrationsWidget();
		qff::MainWindow *fwk = qff::MainWindow::frameWork();
		connect(fwk->plugin("Event"), SIGNAL(dbEventNotify(QString, QVariant)), rw, SLOT(onDbEvent(QString, QVariant)));
		m_registrationsDockWidget->setWidget(rw);
		rw->reload();
		//m_registrationsDockWidget->loadPersistentSettingsRecursively();
	}
}


qf::core::model::SqlTableModel* CompetitorsPlugin::registrationsModel()
{
	if(!m_registrationsModel) {
		m_registrationsModel = new qf::core::model::SqlTableModel(this);
		m_registrationsModel->addColumn("competitorName", tr("Name"));
		m_registrationsModel->addColumn("registration", tr("Reg"));
		m_registrationsModel->addColumn("licence", tr("Lic"));
		m_registrationsModel->addColumn("siId", tr("SI"));
		//m_registrationsModel->addColumn("fistName");
		//m_registrationsModel->addColumn("lastName");
		//m_registrationsModel->addColumn("nameSearchKey");
		qfs::QueryBuilder qb;
		qb.select2("registrations", "firstName, lastName, licence, registration, siId, nameSearchKey")
				.select("COALESCE(lastName, '') || ' ' || COALESCE(firstName, '') AS competitorName")
				.from("registrations")
				.orderBy("lastName, firstName");
		m_registrationsModel->setQueryBuilder(qb);
		m_registrationsModel->reload();
	}
	return m_registrationsModel;
}

}
