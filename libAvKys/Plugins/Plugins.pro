# Webcamoid, webcam capture application.
# Copyright (C) 2011-2016  Gonzalo Exequiel Pedone
#
# Webcamoid is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Webcamoid is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Webcamoid. If not, see <http://www.gnu.org/licenses/>.
#
# Web-Site: http://webcamoid.github.io/

TEMPLATE = subdirs

CONFIG += ordered

win32: SUBDIRS += VirtualCamera/src/dshow/VirtualCameraFilter


###
SUBDIRS_REMOVED = \
    Cartoon \
    ChangeHSL \
    Charify \
    Cinema \
    ColorFilter \
    ColorReplace \
    ColorTap \
    ColorTransform \
    Convolve \
    DelayGrab \
    Denoise \
    DesktopCapture \
    Dice \
    Distort \
    Dizzy \
    Edge \
    Emboss \
    Equalize \
    FaceDetect \
    FalseColor \
    Fire \
    FrameOverlap


SUBDIRS_REMOVED2 = \
    Halftone \
    Hypnotic \
    Implode \
    Invert \
    Life \
    Matrix \
    MatrixTransform \
    Multiplex \
    MultiSink \
    MultiSrc \
    Nervous \
    Normalize \
    OilPaint \
    Photocopy \
    Pixelate \
    PrimariesColors \
    Probe \
    Quark \
    Radioactive \
    Ripple \
    RtPts \
    ScanLines \
    Scroll \
    Shagadelic \
    Swirl \
    Temperature

SUBDIRS += \
    ACapsConvert \
    Aging \
    AudioDevice \
    Bin \
    Blur \
   \
    GrayScale \
    \
    VideoCapture \
    Vignette \
    VirtualCamera \
    Warhol \
    Warp \
    Wave
