#include "messages.h"

Messages Messages::messages;

Messages::Messages()
{
    InitMessages();
}

void Messages::LoadFromCIni(CIniFile* file)
{
    m_iniFile = file;
    QStringList ls = file->getStringList("ignore_messages");
    for (QString s: ls) {
        int i = s.toInt();
        m_messages[i].m_ignore = true;
    }

}

void Messages::InitMessages()
{
    m_messages[DASM_COMPILER_ERROR] = Message(DASM_COMPILER_ERROR, Message::Error, "Oops! Seems like you encountered a DASM compiler error. This typically "
                                   "means that the author of TRSE did something wrong. You can either change some "
                                   "parts of your previous code, or nag the programmer about this bug. If you know any assember, "
                                   "you can always add a comment on the faulty line and try to debug for yourself.");


    m_messages[MEMORY_OVERLAP_ERROR] = Message(MEMORY_OVERLAP_ERROR, Message::Error, "Your program has overlapping memory configurations. A future version of TRSE will include a memory visualization "
                                   "tool, but for now you need to manually reorganize your code blocks and include files.");

    m_messages[NO_DASM] = Message(NO_DASM, Message::Error, "You need to download the Dasm assembler (http://dasm-dillon.sourceforge.net/) and specify the location of dasm.exe in the TRSE settings panel.");
    m_messages[NO_EMULATOR] = Message(NO_EMULATOR, Message::Error,
                                      "You need to download a c64 emulator (such as Vice) and specify the location in the TRSE settings panel.");

}
void Messages::DisplayMessage(int message)
{
    Message m = m_messages[message];

    if (m.m_ignore)
        return;
    DialogMessage* dm = new DialogMessage();

    QImage img;
    if (m.m_severity==Message::Error)
        img.load(":resources/images/friday.png");
    if (m.m_severity==Message::Msg)
        img.load(":resources/images/creatures.png");

    dm->SetMessage(m.m_message, img);
    dm->exec();
    if (dm->ignoreIsChecked()) {
        m_iniFile->addStringList("ignore_messages", QString::number(message), true);
        m_messages[message].m_ignore = true;
    }


    delete dm;

}
