package com.javaclient;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Arrays;
import java.util.concurrent.TimeUnit;

public class MyJavaClient implements IEnvService {

    /**
     * Connects to the server and asks one question.
     * @param _query : the question to be sent to the server.
     * @return server response.
     */
    public String getValueFromServer(String _query){
        String retVal = "";
        int PORT = 4949;
        String HOST = "127.0.0.1";
        try{
            Socket sock = new Socket(HOST, PORT);
            OutputStream out = sock.getOutputStream();
            InputStream in = sock.getInputStream();
            int data;
            StringBuffer line = new StringBuffer();
            out.write(_query.getBytes());
            out.flush();
//            TimeUnit.MILLISECONDS.sleep(100);
            while ((data = in.read() ) > -1) {
//                data = in.read();
                line.append((char) data);
                if((char)data == '#'){
                    break;
                }
            }
            String stringLine = line.toString();
            // remove # at the end
            boolean endsWithHash = stringLine.substring(stringLine.length() - 1).equals("#");
            if(endsWithHash){
                retVal = stringLine.substring(0, stringLine.length() - 1);
            } else{
                retVal = stringLine;
            }
            out.write("break".getBytes());
            out.close();
            in.close();
            sock.close();
        } catch (IOException _e) {
            _e.printStackTrace();
        }
        return retVal;
    }

    /**
     * @return string array of available sensors, i.e: air;light;noise
     */
    @Override
    public String[] requestEnvironmentDataTypes() {
        // line should be like: air;light;noise
        String res = getValueFromServer("getSensortypes()#");
        String[] ret = res.split(";");
        return ret;
    }

    /**
     * Asks the server for the value of a specific sensor in the current time.
     * @param _type : the needed sensor.
     * @return EnvData of the sensor, timestamp, and values.
     */
    @Override
    public EnvData requestEnvironmentData(String _type) {
        String sensorQuery = "getSensor(" + _type + ")#";
        // line should be like: Wed Nov 18 11:00:54 2020|906;684#
        String response = getValueFromServer(sensorQuery);
        String res[] = response.split("\\|");
        String sensorName = _type;
        String timestamp = res[0];
        int[] sensorValues = Arrays.stream(res[1].split(";")).mapToInt(Integer::parseInt).toArray();
        EnvData ret = new EnvData(timestamp, sensorName, sensorValues);
        return ret;
    }

    /**
     * Asks the server for all sensors values.
     * @return EnvData array of all sensors.
     */
    @Override
    public EnvData[] requestAll() {
        EnvData[] ret = new EnvData[3];
        // Wed Nov 18 11:09:12 2020|air;2;|light;17;|noise;409;106;113;799
        String sensors[] = getValueFromServer("getAllSensors()#").split("\\|");
        // Wed Nov 18 11:09:12 2020   air;2;   light;17;   noise;409;106;113;799
        String timestamp = sensors[0];
        for(int i=1;i<sensors.length;i++){
            String sensor = sensors[i];
            boolean endsWithSColon = sensor.substring(sensor.length() - 1).equals(";");
            if(endsWithSColon){
                sensor = sensor.substring(0,sensor.length() - 1);
            }
            // air;2    light;17    noise;409;106;113;799
            int valueStart = sensor.indexOf(";");
            String sensorName = sensor.substring(0, valueStart);
            // air ; 2    light ; 17    noise ; 409;106;113;799
            String values = sensor.substring(valueStart+1);
            int[] sensorValues = Arrays.stream(values.split(";")).mapToInt(Integer::parseInt).toArray();
            // air  2    light  17    noise    409 106 113 799
            EnvData sensorData = new EnvData(timestamp, sensorName, sensorValues);
            ret[i-1] = sensorData;
        }
        return ret;
    }
}
