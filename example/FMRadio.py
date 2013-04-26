#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: FM radio to test FCDPROplus
# Author: dl1ksv
# Generated: Fri Apr 26 16:48:00 2013
##################################################

from PyQt4 import Qt
from gnuradio import audio
from gnuradio import blks2
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.gr import firdes
from gnuradio.qtgui import qtgui
from optparse import OptionParser
import PyQt4.Qwt5 as Qwt
import fcdproplus
import sip
import sys

class FMRadio(gr.top_block, Qt.QWidget):

	def __init__(self):
		gr.top_block.__init__(self, "FM radio to test FCDPROplus")
		Qt.QWidget.__init__(self)
		self.setWindowTitle("FM radio to test FCDPROplus")
		self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
		self.top_scroll_layout = Qt.QVBoxLayout()
		self.setLayout(self.top_scroll_layout)
		self.top_scroll = Qt.QScrollArea()
		self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
		self.top_scroll_layout.addWidget(self.top_scroll)
		self.top_scroll.setWidgetResizable(True)
		self.top_widget = Qt.QWidget()
		self.top_scroll.setWidget(self.top_widget)
		self.top_layout = Qt.QVBoxLayout(self.top_widget)
		self.top_grid_layout = Qt.QGridLayout()
		self.top_layout.addLayout(self.top_grid_layout)


		##################################################
		# Variables
		##################################################
		self.samp_rate = samp_rate = 192000
		self.Volume = Volume = 1
		self.RxFrequency = RxFrequency = 94300
		self.PPM = PPM = 0
		self.Mixergain = Mixergain = 1
		self.Lna = Lna = 1
		self.Ifgain = Ifgain = 0

		##################################################
		# Blocks
		##################################################
		self._Volume_layout = Qt.QHBoxLayout()
		self._Volume_layout.addWidget(Qt.QLabel("Volume"+": "))
		self._Volume_counter = Qwt.QwtCounter()
		self._Volume_counter.setRange(1, 50, 1)
		self._Volume_counter.setNumButtons(2)
		self._Volume_counter.setMinimumWidth(45)
		self._Volume_counter.setValue(self.Volume)
		self._Volume_layout.addWidget(self._Volume_counter)
		self._Volume_counter.valueChanged.connect(self.set_Volume)
		self.top_grid_layout.addLayout(self._Volume_layout, 0,5,1,1)
		self._RxFrequency_layout = Qt.QHBoxLayout()
		self._RxFrequency_layout.addWidget(Qt.QLabel("Set Frequency "+": "))
		self._RxFrequency_counter = Qwt.QwtCounter()
		self._RxFrequency_counter.setRange(3000, 100000, 1)
		self._RxFrequency_counter.setNumButtons(2)
		self._RxFrequency_counter.setMinimumWidth(200)
		self._RxFrequency_counter.setValue(self.RxFrequency)
		self._RxFrequency_layout.addWidget(self._RxFrequency_counter)
		self._RxFrequency_counter.valueChanged.connect(self.set_RxFrequency)
		self.top_grid_layout.addLayout(self._RxFrequency_layout, 0,0,1,1)
		self._PPM_layout = Qt.QHBoxLayout()
		self._PPM_layout.addWidget(Qt.QLabel("set PPM"+": "))
		self._PPM_counter = Qwt.QwtCounter()
		self._PPM_counter.setRange(-100, 100, 1)
		self._PPM_counter.setNumButtons(2)
		self._PPM_counter.setMinimumWidth(40)
		self._PPM_counter.setValue(self.PPM)
		self._PPM_layout.addWidget(self._PPM_counter)
		self._PPM_counter.valueChanged.connect(self.set_PPM)
		self.top_grid_layout.addLayout(self._PPM_layout, 0,6,1,1)
		self._Mixergain_options = (0, 1, )
		self._Mixergain_labels = ("Off", "On", )
		self._Mixergain_tool_bar = Qt.QToolBar(self)
		self._Mixergain_tool_bar.addWidget(Qt.QLabel("Mixergain"+": "))
		self._Mixergain_combo_box = Qt.QComboBox()
		self._Mixergain_tool_bar.addWidget(self._Mixergain_combo_box)
		for label in self._Mixergain_labels: self._Mixergain_combo_box.addItem(label)
		self._Mixergain_callback = lambda i: self._Mixergain_combo_box.setCurrentIndex(self._Mixergain_options.index(i))
		self._Mixergain_callback(self.Mixergain)
		self._Mixergain_combo_box.currentIndexChanged.connect(
			lambda i: self.set_Mixergain(self._Mixergain_options[i]))
		self.top_grid_layout.addWidget(self._Mixergain_tool_bar, 0,2,1,1)
		self._Lna_options = (0, 1, )
		self._Lna_labels = ("Off", "On", )
		self._Lna_tool_bar = Qt.QToolBar(self)
		self._Lna_tool_bar.addWidget(Qt.QLabel("Lna"+": "))
		self._Lna_combo_box = Qt.QComboBox()
		self._Lna_tool_bar.addWidget(self._Lna_combo_box)
		for label in self._Lna_labels: self._Lna_combo_box.addItem(label)
		self._Lna_callback = lambda i: self._Lna_combo_box.setCurrentIndex(self._Lna_options.index(i))
		self._Lna_callback(self.Lna)
		self._Lna_combo_box.currentIndexChanged.connect(
			lambda i: self.set_Lna(self._Lna_options[i]))
		self.top_grid_layout.addWidget(self._Lna_tool_bar, 0,1,1,1)
		self._Ifgain_layout = Qt.QHBoxLayout()
		self._Ifgain_layout.addWidget(Qt.QLabel("Ifgain"+": "))
		self._Ifgain_counter = Qwt.QwtCounter()
		self._Ifgain_counter.setRange(0, 59, 1)
		self._Ifgain_counter.setNumButtons(2)
		self._Ifgain_counter.setMinimumWidth(45)
		self._Ifgain_counter.setValue(self.Ifgain)
		self._Ifgain_layout.addWidget(self._Ifgain_counter)
		self._Ifgain_counter.valueChanged.connect(self.set_Ifgain)
		self.top_grid_layout.addLayout(self._Ifgain_layout, 0,3,1,1)
		self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
			1024, #size
			samp_rate/4, #bw
			"QT GUI Plot", #name
			1 #number of inputs
		)
		self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.pyqwidget(), Qt.QWidget)
		self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_win, 1,4,1,3)
		self.qtgui_sink_x_0 = qtgui.sink_c(
			1024, #fftsize
			firdes.WIN_BLACKMAN_hARRIS, #wintype
			RxFrequency*1000, #fc
			samp_rate, #bw
			"QT GUI Plot", #name
			True, #plotfreq
			True, #plotwaterfall
			False, #plottime
			False, #plotconst
		)
		self.qtgui_sink_x_0.set_update_time(1.0 / 10)
		self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.pyqwidget(), Qt.QWidget)
		self.top_grid_layout.addWidget(self._qtgui_sink_x_0_win, 1,0,1,4)
		self.low_pass_filter_1 = gr.fir_filter_fff(4, firdes.low_pass(
			1, samp_rate, 16000, 300, firdes.WIN_HAMMING, 6.76))
		self.low_pass_filter_0 = gr.fir_filter_ccf(1, firdes.low_pass(
			5, samp_rate, 40000, 300, firdes.WIN_HAMMING, 6.76))
		self.gr_quadrature_demod_cf_0 = gr.quadrature_demod_cf(1)
		self.gr_multiply_const_vxx_0 = gr.multiply_const_vff((Volume, ))
		self.fcdproplus_fcdproplus_0 = fcdproplus.fcdproplus("",1000)
		self.fcdproplus_fcdproplus_0.set_lna(Lna)
		self.fcdproplus_fcdproplus_0.set_mixer_gain(Mixergain)
		self.fcdproplus_fcdproplus_0.set_if_gain(int(Ifgain))
		self.fcdproplus_fcdproplus_0.set_freq_corr(int( PPM))
		self.fcdproplus_fcdproplus_0.set_freq(RxFrequency)
		  
		self.blks2_fm_deemph_0 = blks2.fm_deemph(fs=samp_rate, tau=25e-6)
		self.audio_sink_0 = audio.sink(48000, "default", True)

		##################################################
		# Connections
		##################################################
		self.connect((self.fcdproplus_fcdproplus_0, 0), (self.qtgui_sink_x_0, 0))
		self.connect((self.fcdproplus_fcdproplus_0, 0), (self.low_pass_filter_0, 0))
		self.connect((self.gr_multiply_const_vxx_0, 0), (self.audio_sink_0, 0))
		self.connect((self.gr_multiply_const_vxx_0, 0), (self.audio_sink_0, 1))
		self.connect((self.gr_multiply_const_vxx_0, 0), (self.qtgui_time_sink_x_0, 0))
		self.connect((self.low_pass_filter_0, 0), (self.gr_quadrature_demod_cf_0, 0))
		self.connect((self.low_pass_filter_1, 0), (self.gr_multiply_const_vxx_0, 0))
		self.connect((self.gr_quadrature_demod_cf_0, 0), (self.blks2_fm_deemph_0, 0))
		self.connect((self.blks2_fm_deemph_0, 0), (self.low_pass_filter_1, 0))


	def get_samp_rate(self):
		return self.samp_rate

	def set_samp_rate(self, samp_rate):
		self.samp_rate = samp_rate
		self.low_pass_filter_0.set_taps(firdes.low_pass(5, self.samp_rate, 40000, 300, firdes.WIN_HAMMING, 6.76))
		self.low_pass_filter_1.set_taps(firdes.low_pass(1, self.samp_rate, 16000, 300, firdes.WIN_HAMMING, 6.76))
		self.qtgui_sink_x_0.set_frequency_range(self.RxFrequency*1000, self.samp_rate)

	def get_Volume(self):
		return self.Volume

	def set_Volume(self, Volume):
		self.Volume = Volume
		self.gr_multiply_const_vxx_0.set_k((self.Volume, ))
		self._Volume_counter.setValue(self.Volume)

	def get_RxFrequency(self):
		return self.RxFrequency

	def set_RxFrequency(self, RxFrequency):
		self.RxFrequency = RxFrequency
		self.qtgui_sink_x_0.set_frequency_range(self.RxFrequency*1000, self.samp_rate)
		self._RxFrequency_counter.setValue(self.RxFrequency)
		self.fcdproplus_fcdproplus_0.set_freq(self.RxFrequency)

	def get_PPM(self):
		return self.PPM

	def set_PPM(self, PPM):
		self.PPM = PPM
		self._PPM_counter.setValue(self.PPM)
		self.fcdproplus_fcdproplus_0.set_freq_corr(int( self.PPM))

	def get_Mixergain(self):
		return self.Mixergain

	def set_Mixergain(self, Mixergain):
		self.Mixergain = Mixergain
		self._Mixergain_callback(self.Mixergain)
		self.fcdproplus_fcdproplus_0.set_mixer_gain(self.Mixergain)

	def get_Lna(self):
		return self.Lna

	def set_Lna(self, Lna):
		self.Lna = Lna
		self._Lna_callback(self.Lna)
		self.fcdproplus_fcdproplus_0.set_lna(self.Lna)

	def get_Ifgain(self):
		return self.Ifgain

	def set_Ifgain(self, Ifgain):
		self.Ifgain = Ifgain
		self._Ifgain_counter.setValue(self.Ifgain)
		self.fcdproplus_fcdproplus_0.set_if_gain(int(self.Ifgain))

if __name__ == '__main__':
	parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
	(options, args) = parser.parse_args()
	qapp = Qt.QApplication(sys.argv)
	tb = FMRadio()
	tb.start()
	tb.show()
	qapp.exec_()
	tb.stop()

