package com.javaclient;

public class EnvData {
    /**
     * EnvData is printed on this shape: <mSensorName>;<mTimeStamp>;<mSensorValues>#
     */
    String mSensorName;
    String mTimestamp;
    int[] mSensorValues;

    public EnvData(String _timestamp, String _sensorName, int[] _sensorValues){
        this.mSensorName = _sensorName;
        this.mSensorValues = _sensorValues;
        this.mTimestamp = _timestamp;
    }

    public String getSensorName(){
        return mSensorName;
    }

    public String getTimestamp(){
        return mTimestamp;
    }

    public int[] getSensorValues(){
        return mSensorValues;
    }

    /**
     * Overrides the way the string is printed.
     * @return String
     */
    @Override
    public String toString(){
        String val = "";
        for (int item : getSensorValues()){
            val+=item+";";
        }
        val = val.substring(0, val.length()-1);
        return getTimestamp() + "|" + getSensorName() + ":" + val + "#";
    }
}
