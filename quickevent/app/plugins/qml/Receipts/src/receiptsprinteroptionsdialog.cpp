#include "receiptsprinteroptionsdialog.h"
#include "ui_receiptsprinteroptionsdialog.h"
#include "receiptsprinteroptions.h"

#include <QPrinterInfo>
#include <QPushButton>

ReceiptsPrinterOptionsDialog::ReceiptsPrinterOptionsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ReceiptsPrinterOptionsDialog)
{
	ui->setupUi(this);
	connect(ui->btGraphicsPrinter, &QPushButton::clicked, [this]() {
		this->ui->stackedWidget->setCurrentIndex(0);
	});
	connect(ui->btCharacterPrinter, &QPushButton::clicked, [this]() {
		this->ui->stackedWidget->setCurrentIndex(1);
	});
	loadPrinters();
}

ReceiptsPrinterOptionsDialog::~ReceiptsPrinterOptionsDialog()
{
	delete ui;
}

void ReceiptsPrinterOptionsDialog::loadPrinters()
{
	ui->cbxGraphicPrinter->addItems(QPrinterInfo::availablePrinterNames());
	QString def = QPrinterInfo::defaultPrinterName();
	ui->cbxGraphicPrinter->setCurrentText(def);
}

void ReceiptsPrinterOptionsDialog::setPrinterOptions(const ReceiptsPrinterOptions &opts)
{
	if(opts.printerType() == (int)ReceiptsPrinterOptions::PrinterType::GraphicsPrinter) {
		ui->btGraphicsPrinter->setChecked(true);
		ui->cbxGraphicPrinter->setCurrentText(opts.graphicsPrinterName());
	}
	else {
		ui->btCharacterPrinter->setChecked(true);
		ui->cbxCharacterPrinterDevice->setCurrentText(opts.characterPrinterDevice());
		ui->cbxCharacterPrinterModel->setCurrentText(opts.characterPrinterModel());
		ui->edCharacterPrinterLineLength->setValue(opts.characterPrinterLineLength());
	}
}

ReceiptsPrinterOptions ReceiptsPrinterOptionsDialog::printerOptions()
{
	ReceiptsPrinterOptions ret;
	if(ui->btGraphicsPrinter->isChecked()) {
		ret.setPrinterType((int)ReceiptsPrinterOptions::PrinterType::GraphicsPrinter);
		ret.setGraphicsPrinterName(ui->cbxGraphicPrinter->currentText());
	}
	else {
		ret.setPrinterType((int)ReceiptsPrinterOptions::PrinterType::CharacterPrinter);
		ret.setCharacterPrinterDevice(ui->cbxCharacterPrinterDevice->currentText());
		ret.setCharacterPrinterModel(ui->cbxCharacterPrinterModel->currentText());
		ret.setCharacterPrinterLineLength(ui->edCharacterPrinterLineLength->value());
	}
	return ret;
}

