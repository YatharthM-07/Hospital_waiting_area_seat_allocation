import sys
import os
import subprocess
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QFormLayout, QLineEdit, QComboBox,
    QPushButton, QLabel, QGridLayout, QHBoxLayout
)
from PyQt5.QtCore import Qt

class HospitalWaitingApp(QMainWindow):
    def __init__(self):  
        super().__init__()  
        self.setWindowTitle("Hospital Waiting Area Seat Allocation")
        self.setGeometry(500, 500, 1200, 1000)
        main_widget = QWidget()
        self.setCentralWidget(main_widget)
        layout = QVBoxLayout(main_widget)

        # Form for patient details
        form_widget = QWidget()
        form_layout = QFormLayout(form_widget)

        # Create inputs with neutral background and black text
        input_style = "background-color: #f0f0f0; color: black; padding: 5px; border: 1px solid #ccc; border-radius: 3px;"
        label_style = "font-weight: bold; color: black;"

        self.id_input = QLineEdit()
        self.id_input.setStyleSheet(input_style)
        self.name_input = QLineEdit()
        self.name_input.setStyleSheet(input_style)
        self.age_input = QLineEdit()
        self.age_input.setStyleSheet(input_style)

        self.gender_input = QComboBox()
        self.gender_input.addItems(["Male", "Female", "Other"])
        self.gender_input.setStyleSheet(input_style)
        self.urgency_input = QComboBox()
        self.urgency_input.addItems(["Low", "Medium", "High"])
        self.urgency_input.setStyleSheet(input_style)
        self.contagion_input = QComboBox()
        self.contagion_input.addItems(["Low", "Medium", "High"])
        self.contagion_input.setStyleSheet(input_style)
        self.special_needs_input = QComboBox()
        self.special_needs_input.addItems(["Yes", "No"])
        self.special_needs_input.setStyleSheet(input_style)

        # Add labels with bold text
        form_layout.addRow(QLabel("ID:"), self.id_input)
        form_layout.itemAt(form_layout.count() - 2).widget().setStyleSheet(label_style)
        form_layout.addRow(QLabel("Name:"), self.name_input)
        form_layout.itemAt(form_layout.count() - 2).widget().setStyleSheet(label_style)
        form_layout.addRow(QLabel("Age:"), self.age_input)
        form_layout.itemAt(form_layout.count() - 2).widget().setStyleSheet(label_style)
        form_layout.addRow(QLabel("Gender:"), self.gender_input)
        form_layout.itemAt(form_layout.count() - 2).widget().setStyleSheet(label_style)
        form_layout.addRow(QLabel("Urgency:"), self.urgency_input)
        form_layout.itemAt(form_layout.count() - 2).widget().setStyleSheet(label_style)
        form_layout.addRow(QLabel("Contagion Risk:"), self.contagion_input)
        form_layout.itemAt(form_layout.count() - 2).widget().setStyleSheet(label_style)
        form_layout.addRow(QLabel("Special Needs:"), self.special_needs_input)
        form_layout.itemAt(form_layout.count() - 2).widget().setStyleSheet(label_style)

        # Submit button with visible colored background
        submit_button = QPushButton("Submit")
        submit_button.setStyleSheet("""
            background-color: #008080;  /* teal */
            color: white;
            font-weight: bold;
            padding: 8px 15px;
            border-radius: 5px;
        """)
        submit_button.clicked.connect(self.submit_data)
        form_layout.addWidget(submit_button)

        layout.addWidget(form_widget)

        # --- NEW: Deletion area ---
        delete_widget = QWidget()
        delete_layout = QHBoxLayout(delete_widget)

        self.delete_id_input = QLineEdit()
        self.delete_id_input.setPlaceholderText("Enter Patient ID to delete")
        self.delete_id_input.setStyleSheet(input_style)
        self.delete_id_input.setFixedWidth(180)  # Slightly narrower as requested

        delete_button = QPushButton("Delete Patient by ID")
        delete_button.setStyleSheet("""
            background-color: #DC143C;  /* crimson */
            color: white;
            font-weight: bold;
            padding: 8px 15px;
            border-radius: 5px;
        """)
        delete_button.clicked.connect(self.delete_patient)

        delete_layout.addWidget(self.delete_id_input)
        delete_layout.addWidget(delete_button)
        layout.addWidget(delete_widget)
        # --------------------------

        # Seat matrix (5x10 grid)
        self.seat_labels = []
        seat_widget = QWidget()
        seat_layout = QGridLayout(seat_widget)
        for i in range(5):
            for j in range(10):
                seat_num = i * 10 + j
                label = QLabel(f"Seat {seat_num}")
                label.setAlignment(Qt.AlignCenter)
                label.setStyleSheet("border: 1px solid black; padding: 10px; background-color: lightgreen;")
                seat_layout.addWidget(label, i, j)
                self.seat_labels.append(label)
        layout.addWidget(seat_widget)

        self.result_label = QLabel("Enter patient details and submit to assign a seat.")
        layout.addWidget(self.result_label)
        self.update_seat_matrix()

    # ... rest of your methods unchanged ...
    # (update_seat_matrix, submit_data, delete_patient)

    def update_seat_matrix(self):
        csv_path = "C:/MyProjects/Hospital_waiting_area_seat_allocation/backend_cpp/data/patients.csv"
        try:
            if not os.path.exists(csv_path):
                os.makedirs(os.path.dirname(csv_path), exist_ok=True)
                with open(csv_path, "w") as f:
                    f.write("id,name,age,gender,urgency,contagion_risk,special_needs,priority_score,seat_number\n")
                return
            seat_occupancy, contagious_seats = [None] * 50, []
            with open(csv_path, "r") as f:
                lines = f.readlines()[1:]  # Skip header
                for line in lines:
                    tokens = line.strip().split(",")
                    if len(tokens) >= 9 and tokens[8]:
                        seat = int(tokens[8])
                        if 0 <= seat < 50:
                            seat_occupancy[seat] = tokens[0]  # String ID
                        if tokens[5] == "High":
                            contagious_seats.append(seat)
            blocked_seats = set()
            for seat in contagious_seats:
                row, col = seat // 10, seat % 10
                if col == 0:  # Left edge
                    if not seat_occupancy[row * 10 + 1]:
                        blocked_seats.add(row * 10 + 1)  # Block right (e.g., 0 -> 1)
                elif col == 9:  # Right edge
                    if not seat_occupancy[row * 10 + 8]:
                        blocked_seats.add(row * 10 + 8)  # Block left (e.g., 9 -> 8)
                else:  # Middle seats
                    if col > 0 and not seat_occupancy[row * 10 + (col - 1)]:
                        blocked_seats.add(row * 10 + (col - 1))  # Block left
                    if col < 9 and not seat_occupancy[row * 10 + (col + 1)]:
                        blocked_seats.add(row * 10 + (col + 1))  # Block right
            for i, label in enumerate(self.seat_labels):
                style = "border: 1px solid black; padding: 10px;"
                text = f"Seat {i}"
                if i in blocked_seats:
                    style += "background-color: lightgrey;"
                    text += " (Blocked)"
                elif seat_occupancy[i]:
                    style += "background-color: red;"
                    text += f" (ID: {seat_occupancy[i]})"
                else:
                    style += "background-color: lightgreen;"
                label.setStyleSheet(style)
                label.setText(text)
        except Exception as e:
            self.result_label.setText(f"Error updating seat matrix: {str(e)}")

    def submit_data(self):
        try:
            id_text = self.id_input.text()
            if not (id_text.isdigit() and self.name_input.text() and self.age_input.text().isdigit()):
                self.result_label.setText("Please enter valid numeric ID, name, and age.")
                return
            csv_path = "C:/MyProjects/Hospital_waiting_area_seat_allocation/backend_cpp/data/patients.csv"
            if os.path.exists(csv_path):
                with open(csv_path, "r") as f:
                    if any(line.startswith(f"{id_text},") for line in f.readlines()[1:]):
                        self.result_label.setText(f"ID {id_text} already exists.")
                        return
            patient_data = {
                "id": id_text, "name": self.name_input.text(), "age": self.age_input.text(),
                "gender": self.gender_input.currentText(), "urgency": self.urgency_input.currentText(),
                "contagion_risk": self.contagion_input.currentText(), "special_needs": self.special_needs_input.currentText(),
                "priority_score": "0.0", "seat_number": ""
            }
            os.makedirs(os.path.dirname(csv_path), exist_ok=True)
            with open(csv_path, "a") as f:
                f.write(f"{patient_data['id']},{patient_data['name']},{patient_data['age']},"
                        f"{patient_data['gender']},{patient_data['urgency']},"
                        f"{patient_data['contagion_risk']},{patient_data['special_needs']},"
                        f"{patient_data['priority_score']},\n")
            exe_path = "C:/MyProjects/Hospital_waiting_area_seat_allocation/backend_cpp/main.exe"
            result = subprocess.run([exe_path, csv_path], capture_output=True, text=True, check=True)
            seat_number = result.stdout.strip()
            self.result_label.setText(f"Assigned Seat: {seat_number}" if seat_number != "-1" else "No seats available.")
            if seat_number != "-1":
                self.update_seat_matrix()
        except (subprocess.CalledProcessError, FileNotFoundError, IOError) as e:
            self.result_label.setText(f"Backend error: {str(e)}")
        except Exception as e:
            self.result_label.setText(f"Unexpected error: {str(e)}")

    def delete_patient(self):
        delete_id = self.delete_id_input.text().strip()
        if not delete_id.isdigit():
            self.result_label.setText("Please enter a valid numeric ID to delete.")
            return
        csv_path = "C:/MyProjects/Hospital_waiting_area_seat_allocation/backend_cpp/data/patients.csv"
        try:
            if not os.path.exists(csv_path):
                self.result_label.setText("CSV file does not exist.")
                return
            with open(csv_path, "r") as f:
                lines = f.readlines()
            header = lines[0]
            records = lines[1:]
            # Filter out record with matching ID
            new_records = [line for line in records if not line.startswith(f"{delete_id},")]
            if len(new_records) == len(records):
                self.result_label.setText(f"No patient found with ID {delete_id}.")
                return
            # Write back filtered records
            with open(csv_path, "w") as f:
                f.write(header)
                f.writelines(new_records)
            self.result_label.setText(f"Patient with ID {delete_id} deleted and seat freed.")
            self.delete_id_input.clear()
            self.update_seat_matrix()
        except Exception as e:
            self.result_label.setText(f"Error deleting patient: {str(e)}")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = HospitalWaitingApp()
    window.show()
    sys.exit(app.exec_())
