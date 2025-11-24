MIDI
1.0
As a Sysex ID or Corporate Member, you’ll have access to more details and specs. Learn more on our Membership page.

MIDI 1.0 is a ubiquitous protocol that allows different musical instruments and devices to communicate with each other using digital messages. MIDI 1.0 was first published in 1983, the outcome of collaboration between various manufacturers.

The first version of MIDI 1.0 defined a core data format for messages and a serial transport using 5 pin DIN connectors. MIDI 1.0 has been expanded with many features and reached far beyind the dreams of the original designers.

MIDI 1.0 remains a core vital standard, and continues to evolve. MIDI 1.0 specifications available here also continue to be crucial documents as foundational components of MIDI 2.0.


MIDI 1.0 Core Specifications
MIDI, the Musical Instrument Digital Interface, was established as a hardware and software specification which would make it possible to exchange information (musical notes, program changes, expression control, etc.) between different musical instruments or other devices such as sequencers, computers, lighting controllers, mixers, etc.

MIDI 1.0 DETAILED SPECIFICATION 537.88 KB
This is 1996 revision of the MIDI 1.0 specification.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\M1_v4-2-1_MIDI_1-0_Detailed_Specification_96-1-4.pdf

MIDI TIME CODE
For device synchronization, MIDI Time Code uses two basic types of messages, described as Quarter Frame and Full.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\RP-004-008_v4-2-1_MIDI_Time_Code_Specification_96-1-4.pdf

MIDI SHOW CONTROL
The purpose of MIDI Show Control is to allow MIDI systems to communicate with and to control dedicated intelligent control equipment in theatrical, live performance, multi-media, audio-visual and similar environments.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\RP-002-014_v1-1-1_MIDI_Show_Control_Specification_96-1-4.pdf

MIDI MACHINE CONTROL
MIDI Machine Control is a general purpose protocol which initially allows MIDI systems to communicate with and to control some of the more traditional audio recording and production systems.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\RP-002-014_v1-1-1_MIDI_Show_Control_Specification_96-1-4.pdf

MPE: MIDI Polyphonic Expression
This specification is designed for MIDI controllers that allow the performer to vary the pitch and timbre of individual notes while playing polyphonically.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\M1-100-UM_v1-1_MIDI_Polyphonic_Expression_Specification.pdf

General MIDI
The General MIDI Specifications (GM 1, GM 2, and GM Lite) define specific features and behaviors for compliant MIDI devices. 

General MIDI 1
This Specification outlines a minimum MIDI configuration of a “General MIDI System” which defines a certain class of MIDI controlled sound generators.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\RP-003_General_MIDI_System_Level_1_Specification_96-1-4_0.1.pdf

General MIDI Lite
The General MIDI Lite specification was developed to meet a particular market requirement.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\GML-v1.pdf

General MIDI 2
General MIDI 2 is a group of extensions made to General MIDI (Level 1) allowing for expanded standardized control of MIDI devices. This increased functionality includes extended sounds sets and additional performance and control parameters.

General MIDI Level 2
The following table lists the major MIDI 1.0 messages in numerical (binary) order.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\General_MIDI_Level_2_07-2-6_1.2a.pdf

MIDI Tuning (Updated Specification)
The MIDI Tuning specification allows the sharing of “microtunings” (user-defined scales other than 12-tone equal temperament) among instruments, and the switching of these tunings during real-time performance.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\MIDI Tuning Updated Specification.pdf

Controller Destination Setting
This proposal enables selecting the destination for Control Change messages, plus Channel Pressure and Polyphonic Key Pressure, using Universal Real Time System Exclusive messages.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca22 Controller Destination SysEx Message.pdf

Key-Based Instrument Controllers
The Key-Based Instrument Controller message is intended to be a standard method of providing individual key-based instruments with the same performance control that is available for Channel-based instruments.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca23 Key-based Instrument Controller SysEx Message.pdf

Global Parameter Control
This proposal allows the editing of global parameters in a device using Universal Real-Time System Exclusive messages.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca23 Key-based Instrument Controller SysEx Message.pdf

Master Fine/Course Tuning
These two new Universal Real Time SysEx messages are additional Device Control messages which control the overall tuning of a device.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca25 Master Fine & Coarse Tuning SysEx Message.pdf

Modulation Depth Range RPN
This proposal defines Registered Parameter Number (RPN) #05 as Modulation Depth Range to be used to scale the effective range of Control Change 1 (Modulation Wheel).
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca26 RPN05 Modulation Depth Range.pdf

MIDI 1.0
Addenda
The following changes/additions became part of the MIDI 1.0 Specification after the “96.1” publication and should be consulted to have a current understanding of MIDI technology.

As a Sysex ID or Corporate Member, you’ll have access to more details and specs. Learn more on our Membership page.

MIDI Implementation Chart version 2
This revised version of the standard MIDI Implementation Chart is designed as a quick reference guide that allows users to identify at a glance which MIDI messages and functions are implemented by the device.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\midi_chart-v2.pdf

Renaming of CC91 and CC93
Due to the fact that so many manufacturers use Control Change #91 and #93 in the same way, they should be renamed according to their common usage.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\rp23.pdf
Sound Controller Defaults (Revised)
CC # 70 through 79 are defined as Sound Controllers. Some have default names, and some are left undefined.
Control Change (CC) #75-78 (Sound Controllers 6-9) have been given default names.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\rp21.pdf
Response to Data Increment/Decrement Controllers
This is a recommendation for expected response to Data Increment (MIDI controller #96) and Data Decrement (MIDI controller #97) MIDI messages.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\rp18.pdf

Response to Reset All Controllers
Defines a consistent response to the Reset All Controllers Channel Mode message (#121)
%MCP_DOCUMENT_ROOT%\Standards\MIDI\rp15.pdf
File Reference SysEx Message
This message allows computer based MIDI devices to access files on a shared file system or network using a URL-style file specification. It allows the sounds in these files to be played using simple MIDI messages.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca18.pdf

Three Dimensional Sound Controllers
This specification defines MIDI Registered Parameter Number (RPN) controllers corresponding to the parameters that would be involved in order to allow synthesizers to render MIDI in 3D, and to allow content authors to create compelling 3D MIDI sequences.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\rp49public.pdf
High Resolution Velocity Prefix
Defines MIDI Continuous Controller 88 (58H) as High Resolution Velocity Prefix to the subsequent Note On / Note Off message.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca31.pdf

Default Pan Formula
Over the course of the development of MIDI, different manufacturers have implemented their own responses to CC#10. In addition, various AMEI/MMA Specification (such as GM2 and DLS2) have recommended response curves that are not all the same.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\rp36.pdf
MIDI Bank Index Offset Extension to File Reference SysEx Message
This is an extension to the File Reference SysEx message (“CA-018”) providing a means of offsetting the mapping information in a DLS file by a MIDI bank index offset.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca28.pdf

Redefinition of RPN01 and RPN02 (Channel Fine/Coarse Tuning)
With the approval of CA-025 (“Master Fine/Coarse Tuning”) there arises a naming conflict with existing Registered Parameter Numbers (RPNs) in the MIDI 1.0 Detailed Specification called Master Tuning.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\rp22.pdf
Sample Dump Size/Rate/Name Extensions
This CA adds an Extended Dump Header, capable of handling sample object sizes up to 32 GB (Gigabytes) with greater precision in calling out the sample rate; a new Extended Loop Point message for longer loops; and a Sample Name Transmission / Request pair, all implemented under the Sample Dump Extensions command tree.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca19.pdf

MIDI Visual Control
This specification defines a way for MIDI to be used for control of visual presentation devices or systems. MIDI has expanded far beyond its original intention of being a control language for musical instruments.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\rp50 MIDI Visual Control.pdf
MIDI Tuning (Updated Specification)
The MIDI Tuning specification allows the sharing of “microtunings” (user-defined scales other than 12-tone equal temperament) among instruments, and the switching of these tunings during real-time performance.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\MIDI Tuning Updated Specification.pdf

Controller Destination Setting
This proposal enables selecting the destination for Control Change messages, plus Channel Pressure and Polyphonic Key Pressure, using Universal Real Time System Exclusive messages.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca22 Controller Destination SysEx Message.pdf
Key-Based Instrument Controllers
The Key-Based Instrument Controller message is intended to be a standard method of providing individual key-based instruments with the same performance control that is available for Channel-based instruments.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca23 Key-based Instrument Controller SysEx Message.pdf

Global Parameter Control
This proposal allows the editing of global parameters in a device using Universal Real-Time System Exclusive messages.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca24 Global Parameter Control SysEx Message.pdf
Master Fine/Course Tuning
These two new Universal Real Time SysEx messages are additional Device Control messages which control the overall tuning of a device.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca25 Master Fine & Coarse Tuning SysEx Message.pdf

Modulation Depth Range RPN
This proposal defines Registered Parameter Number (RPN) #05 as Modulation Depth Range to be used to scale the effective range of Control Change 1 (Modulation Wheel).
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca26 RPN05 Modulation Depth Range.pdf
5 Pin DIN Electrical Specs
The MIDI 1.0 Specification includes an Electrical Specification which uses a 5-Pin DIN connector and 5 Volt electronics as was common at that time.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\ca33 5 PIn DIN Electrical Spec.pdf

General MIDI
Level 1 Developer Guidelines
This document was commissioned by the MIDI Manufacturers Association in 1995 to help developers of General MIDI System Level 1 (GM Level 1) products determine how to make their products compatible with as many other GM products as possible.
%MCP_DOCUMENT_ROOT%\Standards\MIDI\file_5f1910382fde6_003_General_MIDI_Level_1_Developer_Guidelines_96-1-4_0.1.pdf