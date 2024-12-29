void GUI_Main::fluentLog(int connectorID, charger_data_t cdata)
{
    // Define log file path
    const std::string directory = "../Logs/fluentLog";
    const std::string filepath = directory + "/fluentLog.txt";
    const std::size_t maxFileSize = 30 * 1024 * 1024; // 200 MB in bytes

    // Create directory if it doesn't exist
    std::filesystem::create_directories(directory);

    // Check the size of the log file
    std::ifstream infile(filepath, std::ios::ate | std::ios::binary);
    if (infile.is_open()) {
        std::size_t fileSize = infile.tellg(); // Get the current size of the file
        infile.close();

        // If the file size exceeds 200 MB, truncate the beginning of the file
        if (fileSize > maxFileSize) {
            std::ifstream file(filepath);
            std::ostringstream buffer;
            buffer << file.rdbuf(); // Read the entire file into a string
            std::string content = buffer.str();
            file.close();

            // Calculate how much to keep (200 MB)
            std::size_t bytesToKeep = fileSize - maxFileSize;
            // Write the last part of the content back to the file
            std::ofstream outfile(filepath, std::ios::binary | std::ios::trunc);
            outfile.write(content.data() + bytesToKeep, content.size() - bytesToKeep);
            outfile.close();
        }
    }

    // Open the file to append each time this function is called
    std::ofstream outfile(filepath, std::ios_base::app);
    if (!outfile.is_open()) {
        std::cerr << "Error opening file for appending." << std::endl;
        return; // Exit if there is an error
    }

    // Generate current time for the log entry
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    // Format the time according to the specified format
    std::ostringstream time_stream;
    std::ostringstream date_stream;
    date_stream << std::put_time(&tm, "%Y-%m-%d");
    time_stream << std::put_time(&tm, "%H:%M:%S");

    // Define fixed and generated values for logging
    std::string charger_id = m_ChargePointIdentifier.toUtf8().constData();
    QString con = QString::number(connectorID) + "-" + m_connectors_cfg[connectorID].label;
    std::string connector_id = con.toStdString();
    // ui->lbl_charge_type_4
    std::string connector_state = std::to_string(connector_cc.at(connectorID)->getModbusState());
    std::string reset_cause = "reset_cause";
    std::string transaction_id = std::to_string(myOcpp.charge_point->getConnectorTransactionID(connectorID));
    std::string connector_error = std::to_string(connector_cc.at(connectorID)->get_fault_code());
    std::string termination_code = std::to_string(mChargeFaults[connectorID]->code);
    std::string availability_code = std::to_string(connector_cc.at(connectorID)->getAvailability(0)); // todo : wrong
    std::string board_command = "board_command"; // todo:
    std::string warning_code = "warning_code"; // todo
    // std::string panel_pc_state = myGUI_States.at(connectorID); // todo
    std::string log_string = "log_string";
    std::string panel_pc_error = "panel_pc_error";
    std::string panel_pc_warning = "panel_pc_warning";
    std::string start_type = connector_cc.at(connectorID)->getLastPacket().toUtf8().constData();
    std::string start_id = std::to_string(myOcpp.charge_point->getConnectorTransactionID(connectorID));
    std::string soc = std::to_string(cdata.ssoc);
    std::string remaining_time = std::to_string(cdata.remaining_time);
    std::string charger_voltage_present_mv = std::to_string(cdata.voltage[0]);
    std::string charger_current_present_mA = std::to_string(cdata.current[0]);
    std::string current = std::to_string(cdata.current[0]);
    std::string charger_power_w = std::to_string(cdata.active_power[0]);
    std::string charging_energy_kWh = std::to_string(cdata.total_energy - cdata.initial_energy);
    std::string total_enery_Wh_H = std::to_string(cdata.total_energy);
    std::string EV_maximum_voltage_limit_V = std::to_string(cdata.max_battery_voltage);
    std::string ev_battery_pack = "ev_batterypack";
    std::string temperature = "N/A-V1"; // todo
    std::string battery_info = connector_cc.at(connectorID)->getBattPacket().toUtf8().constData();
    std::string measure_direction = std::to_string(cdata.measure_direction);
    std::string panel_pc_status_and_log = "panel_pc_status_and_log";
    std::string iap_application_version = QString::fromStdString(myOcpp.config->stackConfig().firmwareVersion()).toUtf8().constData();
    std::string ev_vin = VidPacketStr[connectorID].toUtf8().constData();
    std::string last_packet = connector_cc.at(connectorID)->getLastPacket().toUtf8().constData();
    std::string station_label = m_Station_Label.toUtf8().constData();
    //,panel_pc_status_and_log

    // Write a single log entry with all necessary data
    outfile << date_stream.str() << "|"
            << time_stream.str() << "|"
            << charger_id << "|"
            << connector_id << "|"
            << transaction_id << "|"
            << availability_code << "|"
            << connector_state << "|"
            << reset_cause << "|"
            << termination_code << "|"
            << soc << "|"
            << remaining_time << "|"
            << charger_voltage_present_mv << "|"
            << charger_current_present_mA << "|"
            << charging_energy_kWh << "|"
            << charger_power_w << "|"
            << EV_maximum_voltage_limit_V << "|"
            << total_enery_Wh_H << "|"
            << measure_direction << "|"
            << last_packet << "|"
            << temperature << "|"
            << iap_application_version << "|"
            << battery_info << "|"
            << panel_pc_status_and_log << "|"
            << station_label
            // << connector_error << "|"
            // << board_command << "|"
            // << panel_pc_state << "|"
            // << log_string << "|"
            // << panel_pc_error << "|"
            // << panel_pc_warning << "|"
            // << start_type << "|"
            // << start_id << "|"
            // << current << "|"
            // << power << "|"
            // << ev_battery_pack << "|"
            // << ev_vin << "|"
            << "\n";

    // Close the file
    outfile.close();

    //

    std::ostringstream json_stream;
    // qDebug() << date_stream.str();
        json_stream << "{"
                    << "\"@timestamp\": \"" << date_stream.str() << "T" << time_stream.str() << "Z\","
                    << "\"charger_id\": \"" << charger_id << "\","
                    << "\"connector_id\": \"" << connector_id << "\","
                    << "\"transaction_id\": \"" << transaction_id << "\","
                    << "\"availability_code\": \"" << availability_code << "\","
                    << "\"connector_state\": \"" << connector_state << "\","
                    << "\"reset_cause\": \"" << reset_cause << "\","
                    << "\"termination_code\": \"" << termination_code << "\","
                    << "\"soc\": " << soc << ","
                    << "\"remaining_time\": " << remaining_time << ","
                    << "\"charger_voltage_present_mv\": " << charger_voltage_present_mv << ","
                    << "\"charger_current_present_mA\": " << charger_current_present_mA << ","
                    << "\"charging_energy_kWh\": " << charging_energy_kWh << ","
                    << "\"charger_power_w\": \"" << charger_power_w << "\","
                    << "\"EV_maximum_voltage_limit_V\": " << EV_maximum_voltage_limit_V << ","
                    << "\"total_energy_Wh_H\": " << total_enery_Wh_H << ","
                    << "\"measure_direction\": " << measure_direction << ","
                    << "\"last_packet\": \"" << last_packet << "\","
                    << "\"temperature\": \"" << temperature << "\","
                    << "\"iap_application_version\": \"" << iap_application_version << "\","
                    << "\"battery_info\": \"" << battery_info << "\","
                    << "\"panel_pc_status_and_log\": \"" << panel_pc_status_and_log << "\","
                    << "\"station_label\": \"" << station_label << "\""
                    << "}";

        std::string json_data = json_stream.str();

        // Construct the curl command
        std::string curl_command = "curl -u \"charger:Ch@rgEr@123456\" -XPOST https://elstc.emapna.com/charger-log/_doc -H 'Content-Type:application/json' -d '" + json_data + "'";

        // Execute the curl command
        std::system(curl_command.c_str());
}
