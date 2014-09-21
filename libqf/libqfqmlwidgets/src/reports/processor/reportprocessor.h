
//
// Author: Frantisek Vacek <fanda.vacek@volny.cz>, (C) 2014
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef QF_QMLWIDGETS_REPORTS_REPORTPROCESSOR_H
#define QF_QMLWIDGETS_REPORTS_REPORTPROCESSOR_H

#include "../../qmlwidgetsglobal.h"
#include "reportitem.h"
#include "reportdocument.h"
#include "reportprocessorcontext.h"

#include <qf/core/utils/treetable.h>
//#include <qf/core/utils/searchdirs.h>
#include <qf/core/assert.h>

#include <QObject>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QPointer>

class QPrinter;
class QQmlEngine;

namespace qf {
namespace qmlwidgets {
namespace reports {

class ReportProcessorItem;
class ReportItemBand;
class ReportItemMetaPaint;
class ReportItemMetaPaintReport;

//! TODO: write class documentation.
class  QFQMLWIDGETS_DECL_EXPORT ReportProcessor : public QObject
{
	Q_OBJECT
public:
	enum ProcessorMode {SinglePage = 1, FirstPage, AllPages};
	typedef QMap<QString, ReportProcessorItem::Image> ImageMap;
public:
	ReportProcessor(QPaintDevice *paint_device, QObject *parent = NULL);
	~ReportProcessor() Q_DECL_OVERRIDE;
protected:
	void makeContext();
	//void readStyleSheet(const QDomElement &el_stylesheet);
public:
	const ReportProcessorContext& context() const {return f_Context;}
public:
	/// vymaze vsechna data vznikla predchozimi kompilacemi
	void reset();
	//--void setReport(const ReportDocument &doc);
	void setReport(const QString &rep_file_name);
	//--ReportDocument report() {return fReport;}
	//--ReportDocument& reportRef() {return fReport;}
	void setData(const qf::core::utils::TreeTable &_data);
	const qf::core::utils::TreeTable& data() const {return f_data;}

	void addImage(const QString key, const ReportProcessorItem::Image &img) {fImageMap[key] = img;}
	const ImageMap& images() const {return fImageMap;}

	bool isDesignMode() const {return m_designMode;}
	void setDesignMode(bool b) {m_designMode = b;}
public:
	void setPaintDevice(QPaintDevice *pd) {fPaintDevice = pd;}
	QPaintDevice* paintDevice() {
		QF_ASSERT_EX(fPaintDevice, "paintDevice cannot be null");
		return fPaintDevice;
	}
	//! Vrati QFontMetricsF pro \a font a \a paintDevice() .
	//! Pokud je paintDevice NULL, vrati fontMetrics pro screen.
	QFontMetricsF fontMetrics(const QFont &font);
	/*
		QColor color(const QString &name_or_def);
		QPen pen(const QString &name_or_def);
		QBrush brush(const QString &name_or_def);
		QFont font(const QString &name_or_def);
		Style style(const QString &name_or_def);
		*/
	static bool isProcessible(const QDomElement &el);
	//! vytvori item pro element a nastavi nektere deefaultni hodnoty atributu, postara se taky o atribut copyAttributesFrom.
	//! Pro vytvoreni kontkretniho itemu pak vola funkci createItem()
	ReportProcessorItem* createProcessibleItem(const QDomElement &el, ReportProcessorItem *parent);
	virtual ReportProcessorItem* createItem(ReportProcessorItem *parent, const QDomElement &el);

	//! cislo stranky, ktera se zrovna zpracovava, pocitaji se od 0.
	int processedPageNo() const {return fProcessedPageNo;}
	/*
		static void appendSearchDir(const QString &path) {f_searchDirs.appendDir(path);}
		static void setSearchDirs(const QStringList &sl) {f_searchDirs.setDirs(sl);}
		/// oddelovac je kvuli windows "::"
		static void setSearchDirs(const QString &s) {f_searchDirs.setDirs(s);}
		*/
	//--qf::core::utils::SearchDirs* searchDirs(bool throw_exc = true);
	/// does not take ownership os \a sd
	//--void setSearchDirs(qf::core::utils::SearchDirs *sd) {f_searchDirs = sd;}
protected:
	virtual ReportProcessorItem::PrintResult processPage(ReportItemMetaPaint *out);
	/// return NULL if such a page does not exist.
	ReportItemMetaPaintFrame *getPage(int page_no);
public:
	virtual void process(ProcessorMode mode = AllPages);
	void print(QPrinter &printer, const QVariantMap &options);

	int pageCount();

	ReportItemMetaPaintReport* processorOutput() {return f_processorOutput;}
	ReportItemReport* documentInstanceRoot();
public:
	/// vlozi do el_body report ve formatu HTML
	virtual void processHtml(QDomElement &el_body);

	void dump();
protected:
	ReportProcessorContext& contextRef() {return f_Context;}

	void fixTableTags(QDomElement &el);
	QDomElement removeRedundantDivs(QDomElement &el);
	QDomElement fixLayoutHtml(QDomElement &el);
signals:
	//! emitovan vzdy, kdyz procesor dokonci dalsi stranku.
	void pageProcessed();
public slots:
	//! prelozi dalsi stranku reportu (takhle delam multithreading, protoze QFont musi bezet v GUI threadu)
	void processSinglePage() {process(SinglePage);}
protected:
	QQmlEngine* qmlEngine(bool throw_exc = true);
private:
	ReportProcessorContext f_Context;

	ImageMap fImageMap;

	QQmlEngine *m_qmlEngine;
	ReportDocument *m_reportDocumentComponent;
	qf::core::utils::TreeTable f_data;
	QPaintDevice *fPaintDevice;
	//! pri prekladu xml reporu vznika strom odpovidajicich ReportProcessorItem objektu a toto je jejich root.
	ReportItemReport *m_documentInstanceRoot;
	//ReportProcessorItem *f_processedItemsRoot;
	//! pri ReportProcessorItem objekty generuji pomoci metody \a printMetaPaint() objekty ReportItemMetaPaint a toto je jejich root.
	ReportItemMetaPaintReport *f_processorOutput;

	int fProcessedPageNo;
	//--qf::core::utils::SearchDirs *f_searchDirs;
	ReportProcessorItem::PrintResult singlePageProcessResult;

	//! v design view je detail zobrazen, i kdyz nejsou data, aby bylo neco videt.
	bool m_designMode;
};

}}}

#endif // QF_QMLWIDGETS_REPORTS_REPORTPROCESSOR_H

