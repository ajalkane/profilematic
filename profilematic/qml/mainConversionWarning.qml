import QtQuick 1.0
import com.nokia.meego 1.0

PageStackWindow {
    id: root
    QueryDialog {
        id: dQuery
        titleText: "ProfileMatic update information"
        message: "Your rules have been converted to new format. "
                 + "The rules should work after conversion roughly as before, "
                 + "but you should make sure."
                 +"\n\n"
                 + "Editing the rules is recommended anyway, as many rules can "
                 + "now be simplified by using end time and the default rule."
        onAccepted: Qt.quit();
        onRejected: Qt.quit();
    }

    Component.onCompleted: {
        dQuery.open();
    }
}
