import QtQuick 1.0
import com.nokia.meego 1.0
import com.nokia.extras 1.0

TimePickerDialog {
    fields: DateTime.Hours | DateTime.Minutes
    hourMode: DateTime.TwentyFourHours
    acceptButtonText: "Ok"
    rejectButtonText: "Cancel"
}
