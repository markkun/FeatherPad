/*
 * Copyright (C) Pedram Pourang (aka Tsu Jan) 2014 <tsujan2000@gmail.com>
 *
 * FeatherPad is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FeatherPad is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "lineedit.h"
#include <QToolButton>
#include <QStyle>
#include <QApplication>

namespace FeatherPad {

LineEdit::LineEdit (QWidget *parent)
    : QLineEdit (parent)
{
    klear = NULL;
    returnOnClear = true;
    setClearButtonEnabled (true);
    QList<QToolButton*> list = findChildren<QToolButton*>();
    if (list.isEmpty()) return;
    QToolButton *clearButton = list.at (0);
    if (clearButton)
    {
        clearButton->setToolTip ("Clear text (Ctrl+K)");
        connect (clearButton, SIGNAL (clicked()), this, SLOT (Klear()));
    }
    connect (this, SIGNAL (editingFinished()), this, SLOT (unfocused()));
    connect (this, SIGNAL (receivedFocus()), this, SLOT (focused()));
}
/*************************/
LineEdit::~LineEdit()
{
    if (klear) delete klear;
}
/*************************/
void LineEdit::focusInEvent (QFocusEvent * ev)
{
    /* first do what QLineEdit does */
    QLineEdit::focusInEvent (ev);
    emit receivedFocus();
}
/*************************/
void LineEdit::Klear()
{
    if (!qobject_cast<QToolButton*>(QObject::sender()))
        clear();
    /* we'll need this for clearing found matches highlighting
       because the compiler won't know that clearButton is a QObject */
    if (returnOnClear)
        returnPressed();
}
/*************************/
void LineEdit::unfocused()
{
    /* filter out pressing of Return or Enter */
    if (hasFocus()) return;

    if (klear)
    {
        disconnect (klear, SIGNAL (activated()), this, SLOT (Klear()));
        delete klear;
        klear = NULL;
    }
}
/*************************/
void LineEdit::focused()
{
    if (klear) return;

    klear = new QShortcut (QKeySequence (tr ("Ctrl+K", "Clear text")), this);
    connect (klear, SIGNAL (activated()), this, SLOT (Klear()));
}

}
