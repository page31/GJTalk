#pragma once
/*--------------------------------------------------------------------------------
* Adaptive Intelligence - Foundation Framework Class Library
* Version 2.00
* 
* Smart Date Structure / Value Type Class
*      Provides a re-defined and enhanced version of the System.DateTime 
* data type.
*------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
* System Imports
*------------------------------------------------------------------------------*/
#include "stdafx.h"

namespace Adaptive
{
	namespace Foundation
	{
		/// <summary>
		/// Provides a re-defined and enhanced version of the <see cref="DateTime"/> 
		/// data type.
		/// </summary>
		/// <remarks>
		/// This class is used to provide the same functionality as the <see cref="DateTime"/> class, with
		/// the added ability to support a nullable and empty state.
		/// </remarks>

		public value class SmartDateTime : public IComparable, IFormattable, IConvertible, ISerializable, 
			IComparable<SmartDateTime>, IEquatable<SmartDateTime>
		{
		public:

			/*--------------------------------------------------------------------------------
			* Constructor / Dispose / Destructor Methods
			*------------------------------------------------------------------------------*/
			#pragma region Constructor / Dispose / Destructor Methods
			/// <summary>
			/// Initializes a new instance of the structure to the specified tick count.
			/// </summary> 
			/// <param name="ticks">The tick count for the date.</param>
			SmartDateTime(__int64 ticks);
			/// <summary>
			/// Initializes a new instance of the structure to the specified tick count.
			/// </summary> 
			/// <param name="ticks">A date and time expressed in 100-nanosecond units.</param>
			SmartDateTime(unsigned __int64 ticks);
			/// <summary>
			/// Initializes a new instance of the structure to a specified number of ticks and to Coordinated Universal Time (UTC) or local time.
			/// </summary> 
			/// <param name="ticks">A date and time expressed in 100-nanosecond units.</param>
			/// <param name="kind">One of the <see cref="DateTimeKind"/> values that indicates whether ticks specifies a local time, 
			/// Coordinated Universal Time (UTC), or neither.</param>
			SmartDateTime(__int64 ticks, DateTimeKind kind);
			/// <summary>
			/// Initializes a new instance of the structure to a specified number of ticks and to Coordinated Universal Time (UTC) or local time.
			/// </summary> 
			/// <param name="ticks">A date and time expressed in 100-nanosecond units.</param>
			/// <param name="kind">One of the <see cref="DateTimeKind"/> values that indicates whether ticks specifies a local time, 
			/// Coordinated Universal Time (UTC), or neither.</param>
			/// <param name="isAmbiguousDest"></param>
			SmartDateTime(__int64 ticks, DateTimeKind kind, bool isAmbiguousDest);
			/// <summary>
			/// Initializes a new instance of the structure from the supplied serialized data.
			/// </summary> 
			/// <param name="info">A reference to a <see cref="SerializationInfo"/> containing the serialization information.</param>
			/// <param name="context">A reference to a <see cref="StreamingContext"/> containing the serialization context.</param>
			SmartDateTime(SerializationInfo^ info, StreamingContext context);
			/// <summary>
			/// Initializes a new instance of the structure to the specified year, month, and day.
			/// </summary> 
			/// <param name="year">The year (1 through 9999).</param>
			/// <param name="month">The month (1 through 12).</param>
			/// <param name="day">The day (1 through the number of days in month).</param>
			SmartDateTime(int year, int month, int day);
			/// <summary>
			/// Initializes a new instance of the structure to the specified year, month, and day for
			/// the specified calendar.
			/// </summary> 
			/// <param name="year">The year (1 through 9999).</param>
			/// <param name="month">The month (1 through 12).</param>
			/// <param name="day">The day (1 through the number of days in month).</param>
			/// <param name="calendar">The <see cref="Calendar"/> that applies to this <b>SmartDateTime</b>.</param>
			SmartDateTime(int year, int month, int day, Calendar^ calendar);
			/// <summary>
			/// Initializes a new instance of the structure to the specified year, month, and day for
			/// the specified calendar.
			/// </summary> 
			/// <param name="year">The year (1 through 9999).</param>
			/// <param name="month">The month (1 through 12).</param>
			/// <param name="day">The day (1 through the number of days in month).</param>
			/// <param name="hour">The hour value.</param>
			/// <param name="minute">The minute value.</param>
			/// <param name="second">The second value.</param>
			SmartDateTime(int year, int month, int day, int hour, int minute, int second);
			/// <summary>
			/// Initializes a new instance of the structure to the specified year, month, and day for
			/// the specified calendar.
			/// </summary> 
			/// <param name="year">The year (1 through 9999).</param>
			/// <param name="month">The month (1 through 12).</param>
			/// <param name="day">The day (1 through the number of days in month).</param>
			/// <param name="hour">The hour value.</param>
			/// <param name="minute">The minute value.</param>
			/// <param name="second">The second value.</param>
			/// <param name="kind">The <see cref="DateTimeKind"/> that applies to this <b>SmartDateTime</b>.</param>
			SmartDateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind);
			/// <summary>
			/// Initializes a new instance of the structure to the specified year, month, and day for
			/// the specified calendar.
			/// </summary> 
			/// <param name="year">The year (1 through 9999).</param>
			/// <param name="month">The month (1 through 12).</param>
			/// <param name="day">The day (1 through the number of days in month).</param>
			/// <param name="hour">The hour value.</param>
			/// <param name="minute">The minute value.</param>
			/// <param name="second">The second value.</param>
			/// <param name="calendar">The <see cref="Calendar"/> that applies to this <b>SmartDateTime</b>.</param>
			SmartDateTime(int year, int month, int day, int hour, int minute, int second, Calendar^ calendar);
			/// <summary>
			/// Initializes a new instance of the structure to the specified year, month, and day for
			/// the specified calendar.
			/// </summary> 
			/// <param name="year">The year (1 through 9999).</param>
			/// <param name="month">The month (1 through 12).</param>
			/// <param name="day">The day (1 through the number of days in month).</param>
			/// <param name="hour">The hour value.</param>
			/// <param name="minute">The minute value.</param>
			/// <param name="second">The second value.</param>
			/// <param name="millisecond">The millisecond value.</param>
			SmartDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond);
			/// <summary>
			/// Initializes a new instance of the structure to the specified year, month, and day for
			/// the specified calendar.
			/// </summary> 
			/// <param name="year">The year (1 through 9999).</param>
			/// <param name="month">The month (1 through 12).</param>
			/// <param name="day">The day (1 through the number of days in month).</param>
			/// <param name="hour">The hour value.</param>
			/// <param name="minute">The minute value.</param>
			/// <param name="second">The second value.</param>
			/// <param name="millisecond">The millisecond value.</param>
			/// <param name="kind">The <see cref="DateTimeKind"/> that applies to this <b>SmartDateTime</b>.</param>
			SmartDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind);
			/// <summary>
			/// Initializes a new instance of the structure to the specified year, month, and day for
			/// the specified calendar.
			/// </summary> 
			/// <param name="year">The year (1 through 9999).</param>
			/// <param name="month">The month (1 through 12).</param>
			/// <param name="day">The day (1 through the number of days in month).</param>
			/// <param name="hour">The hour value.</param>
			/// <param name="minute">The minute value.</param>
			/// <param name="second">The second value.</param>
			/// <param name="millisecond">The millisecond value.</param>
			/// <param name="calendar">The <see cref="Calendar"/> that applies to this <b>SmartDateTime</b>.</param>
			SmartDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, Calendar^ calendar);
			/// <summary>
			/// Initializes a new instance of the structure to the specified year, month, and day for
			/// the specified calendar.
			/// </summary> 
			/// <param name="year">The year (1 through 9999).</param>
			/// <param name="month">The month (1 through 12).</param>
			/// <param name="day">The day (1 through the number of days in month).</param>
			/// <param name="hour">The hour value.</param>
			/// <param name="minute">The minute value.</param>
			/// <param name="second">The second value.</param>
			/// <param name="millisecond">The millisecond value.</param>
			/// <param name="calendar">The <see cref="Calendar"/> that applies to this <b>SmartDateTime</b>.</param>
			/// <param name="kind">The <see cref="DateTimeKind"/> that applies to this <b>SmartDateTime</b>.</param>
			SmartDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, Calendar^ calendar, DateTimeKind kind);
			/// <summary>
			/// Initializes a new instance of the structure to the specified year, month, and day for
			/// the specified calendar.
			/// </summary> 
			/// <param name="originalValue">The original <see cref="DateTime"/> value to copy.</param>
			SmartDateTime(DateTime originalValue);
			/// <summary>
			/// Initializes a new instance of the structure to the specified year, month, and day for
			/// the specified calendar.
			/// </summary> 
			/// <param name="isNull">A value indicating whether the <b>SmartDateTime</b> represents a null value.</param>
			SmartDateTime(bool isNull);
			/// <summary>
			/// Initializes a new instance of the structure to the specified year, month, and day for
			/// the specified calendar.
			/// </summary> 
			/// <param name="isNull">A value indicating whether the <b>SmartDateTime</b> represents a null value.</param>
			/// <param name="isEmpty">A value indicating whether the <b>SmartDateTime</b> represents an empty value.</param>
			SmartDateTime(bool isNull, bool isEmpty);
			/// <summary>
			/// Initializes a new instance of the structure to the specified year, month, and day for
			/// the specified calendar.
			/// </summary> 
			/// <param name="originalValue">An object to translate into a date/time value.</param>
			SmartDateTime(Object^ originalValue);
			/// <summary>
			/// Initializes a new instance of the structure to the specified year, month, and day for
			/// the specified calendar.
			/// </summary> 
			/// <param name="originalValue">An object to translate into a date/time value.</param>
			/// <param name="isNull">A value indicating whether the <b>SmartDateTime</b> represents a null value.</param>
			SmartDateTime(DateTime originalValue, bool isNull);
			#pragma endregion

			/*--------------------------------------------------------------------------------
			* Public Properties
			*------------------------------------------------------------------------------*/
#pragma region Public Properties
			/// <summary>
			/// Gets the null value for <b>SmartDateTime</b>.
			/// </summary>
			/// <value>
			/// A <see cref="SmartDateTime"/> structure set to Null.
			/// </value>
			static property SmartDateTime Null { SmartDateTime get(); }
			/// <summary>
			/// Gets the empty value for <b>SmartDateTime</b>.
			/// </summary>
			/// <value>
			/// A <see cref="SmartDateTime"/> structure set to Empty.
			/// </value>
			static property SmartDateTime Empty { SmartDateTime get(); }
			/// <summary>
			/// Gets the largest value for <b>SmartDateTime</b> possible.
			/// </summary>
			/// <value>
			/// The largest <see cref="SmartDateTime"/> value possible.
			/// </value>
			static property SmartDateTime MaxValue { SmartDateTime get(); }
			/// <summary>
			/// Gets the smallest value for <b>SmartDateTime</b> possible.
			/// </summary>
			/// <value>
			/// The smallest <see cref="SmartDateTime"/> value possible.
			/// </value>
			static property SmartDateTime MinValue { SmartDateTime get(); }
			/// <summary>
			/// Gets the date component of this instance.
			/// </summary>
			/// <value>
			/// A new <see cref="SmartDateTime"/> with the same date as this instance, and the time value set to 12:00:00 midnight (00:00:00).
			/// </value>
			property SmartDateTime Date { SmartDateTime get(); }
			property int Day { int get(); }
			property DayOfWeek XDayOfWeek { DayOfWeek get(); }
			property int DayOfYear { int get(); }
			property int Hour { int get(); }
			property unsigned __int64 InternalKind { unsigned __int64 get(); }
			property __int64 InternalTicks { __int64 get(); }
			property DateTimeKind Kind { DateTimeKind get(); }
			property int Millisecond { int get(); }
			property int Minute { int get(); }
			property int Month { int get(); }
			static property SmartDateTime Now { SmartDateTime get(); }
			property int Second { int get(); }
			property __int64 Ticks { __int64 get(); }
			property TimeSpan TimeOfDay { TimeSpan get(); }
			property static SmartDateTime Today { SmartDateTime get(); }
			property static SmartDateTime UtcNow { SmartDateTime get(); }
			property int Year { int get(); }
			property bool IsNull { bool get(); }
			property bool IsEmpty { bool get(); }
			/// <summary>
			/// Gets a database-friendly version of the date value.
			/// </summary>
			/// <remarks>
			/// <para>
			/// If the SmartDate contains an empty date, this returns <see cref="DBNull"/>.
			/// Otherwise the actual date value is returned as type Date.
			/// </para><para>
			/// This property is very useful when setting parameter values for
			/// a Command object, since it automatically stores null values into
			/// the database for empty date values.
			/// </para><para>
			/// When you also use the SafeDataReader and its GetSmartDate method,
			/// you can easily read a null value from the database back into a
			/// SmartDate object so it remains considered as an empty date value.
			/// </para>
			/// </remarks>
			property Object^ DBValue { Object^ get(); }

#pragma endregion

			/*--------------------------------------------------------------------------------
			* Public Methods / Functions
			*------------------------------------------------------------------------------*/
#pragma region Public Methods / Functions
			/// <summary>
			/// 
			/// </summary>
			/// <remarks>
			/// 
			/// </remarks>
			SmartDateTime Add(TimeSpan value);
			SmartDateTime Add(SmartDateTime value);
			SmartDateTime Add(DateTime value);
			SmartDateTime Add(double value, int scale);
			SmartDateTime AddDays(double value);
			SmartDateTime AddHours(double value);
			SmartDateTime AddMilliseconds(double value);
			SmartDateTime AddMinutes(double value);
			SmartDateTime AddMonths(int value);
			SmartDateTime AddSeconds(double value);
			SmartDateTime AddTicks(__int64 value);
			SmartDateTime AddYears(int value);
			static int Compare(SmartDateTime leftDate, SmartDateTime rightDate);
			static int Compare(SmartDateTime leftDate, DateTime rightDate);
			virtual int CompareTo(SmartDateTime value);
			int CompareTo(DateTime value);
			virtual int CompareTo(Object^ value);
			static int DaysInMonth(int year, int month);
			static __int64 DateToTicks(int year, int month, int day);
			static __int64 DoubleDateToTicks(double value);
			bool Equals(DateTime value);
			virtual bool Equals(SmartDateTime value);
			virtual bool Equals(Object^ value) override;
			static bool Equals(SmartDateTime t1, SmartDateTime t2);
			static DateTime FromBinary(__int64 dateData);
			static DateTime FromBinaryRaw(__int64 dateData);
			static DateTime FromFileTime(__int64 fileTime);
			static DateTime FromFileTimeUtc(__int64 fileTime);
			static DateTime FromOADate(double d);
			int GetDatePart(int part);
			array<String^>^ GetDateTimeFormats();
			array<String^>^ GetDateTimeFormats(char format);
			array<String^>^ GetDateTimeFormats(IFormatProvider^ provider);
			array<String^>^ GetDateTimeFormats(char format, IFormatProvider^ provider);
			virtual int GetHashCode() override;
			static __int64 GetSystemTimeAsFileTime();
			bool IsAmbiguousDaylightSavingTime();
			bool IsDaylightSavingTime();
			static bool IsLeapYear(int year);
			static SmartDateTime operator +(SmartDateTime d, TimeSpan t);
			static bool operator ==(SmartDateTime d1, SmartDateTime d2);
			static bool operator >(SmartDateTime t1, SmartDateTime t2);
			static bool operator >=(SmartDateTime t1, SmartDateTime t2);
			static bool operator !=(SmartDateTime d1, SmartDateTime d2);
			static bool operator <(SmartDateTime t1, SmartDateTime t2);
			static bool operator <(SmartDateTime t1, DateTime t2);
			static bool operator <=(SmartDateTime t1, SmartDateTime t2);
			static TimeSpan operator -(SmartDateTime d1, DateTime d2);
			static TimeSpan operator -(SmartDateTime d1, TimeSpan d2);
			static TimeSpan operator -(SmartDateTime d, SmartDateTime t);
			static SmartDateTime Parse(String^ s);
			static SmartDateTime Parse(String^ s, IFormatProvider^ provider);
			static SmartDateTime Parse(String^ s, IFormatProvider^ provider, DateTimeStyles styles);
			static SmartDateTime ParseExact(String^ s, String^ format, IFormatProvider^ provider);
			static SmartDateTime ParseExact(String^ s, String^ format, IFormatProvider^ provider, DateTimeStyles style);
			static SmartDateTime ParseExact(String^ s, array<String^>^ formats, IFormatProvider^ provider, DateTimeStyles style);
			static SmartDateTime SpecifyKind(DateTime value, DateTimeKind kind);
			TimeSpan Subtract(SmartDateTime value);
			SmartDateTime Subtract(TimeSpan value);
			virtual bool ToBoolean(IFormatProvider^ provider);
			virtual Byte ToByte(IFormatProvider^ provider);
			virtual wchar_t ToChar(IFormatProvider^ provider);
			virtual DateTime ToDateTime(IFormatProvider^ provider);
			virtual Decimal ToDecimal(IFormatProvider^ provider);
			virtual double ToDouble(IFormatProvider^ provider);
			virtual short ToInt16(IFormatProvider^ provider);
			virtual int ToInt32(IFormatProvider^ provider);
			virtual __int64 ToInt64(IFormatProvider^ provider);
			virtual SByte ToSByte(IFormatProvider^ provider);
			virtual float ToSingle(IFormatProvider^ provider);
			virtual Object^ ToType(Type^ type, IFormatProvider^ provider);
			virtual unsigned short ToUInt16(IFormatProvider^ provider);
			virtual unsigned int ToUInt32(IFormatProvider^ provider);
			virtual unsigned __int64 ToUInt64(IFormatProvider^ provider);
			[SecurityPermission(SecurityAction::LinkDemand, Flags=SecurityPermissionFlag::SerializationFormatter)]
			virtual void GetObjectData(SerializationInfo^ info, StreamingContext context);
			static double TicksToOADate(__int64 value);
			static __int64 TimeToTicks(int hour, int minute, int second);
			virtual TypeCode GetTypeCode();
			__int64 ToBinary();
			__int64 ToBinaryRaw();
			__int64 ToFileTime();
			__int64 ToFileTimeUtc();
			SmartDateTime ToLocalTime();
			String^ ToLongDateString();
			String^ ToLongTimeString();
			double ToOADate();
			String^ ToShortDateString();
			String^ ToShortTimeString();
			virtual String^ ToString() override;
			virtual String^ ToString(IFormatProvider^ provider);
			String^ ToString(String^ format);
			virtual String^ ToString(String^ format, IFormatProvider^ provider);
			DateTime ToUniversalTime();
			static bool TryCreate(int year, int month, int day, int hour, int minute, int second, int millisecond, SmartDateTime^ result);
			static bool TryParse(String^ s, SmartDateTime^ result);
			static bool TryParse(String^ s, IFormatProvider^ provider, DateTimeStyles styles, SmartDateTime^ result);
			static bool TryParseExact(String^ s, array<String^>^ formats, IFormatProvider^ provider, DateTimeStyles style, SmartDateTime^ result);
			static bool TryParseExact(String^ s, String^ format, IFormatProvider^ provider, DateTimeStyles style, SmartDateTime^ result);


#pragma endregion

		protected:

			/*--------------------------------------------------------------------------------
			* Protected Methods / Event Methods
			*------------------------------------------------------------------------------*/
#pragma region Protected Methods / Event Methods
#pragma endregion

		private:
			/*--------------------------------------------------------------------------------
			* Private Constants Declarations
			*------------------------------------------------------------------------------*/
#pragma region Private Constants Declarations
			static String^ DateDataField = "dateData";
			static String^ TicksField = "ticks";
			static const int DatePartDay = 3;
			static const int DatePartDayOfYear = 1;
			static const int DatePartMonth = 2;
			static const int DatePartYear = 0;
			static const int DaysPer100Years = 0x8eac;
			static const int DaysPer400Years = 0x23ab1;
			static const int DaysPer4Years = 0x5b5;
			static const int DaysPerYear = 0x16d;
			static const int DaysTo10000 = 0x37b9db;
			static const int DaysTo1601 = 0x8eac4;
			static const int DaysTo1899 = 0xa9559;
			static const __int64 DoubleDateOffset = 0x85103c0cb83c000L;
			static const __int64 FileTimeOffset = 0x701ce1722770000L;
			static const unsigned __int64 FlagsMask = 13835058055282163712L;
			static const unsigned __int64 KindLocal = 9223372036854775808L;
			static const unsigned __int64 KindLocalAmbiguousDst = 13835058055282163712L;
			static const int KindShift = 0x3e;
			static const unsigned __int64 KindUnspecified = 0L;
			static const unsigned __int64 KindUtc = 0x4000000000000000L;
			static const unsigned __int64 LocalMask = 9223372036854775808L;
			static const __int64 MaxMillis = 0x11efae44cb400L;
			static  const __int64 MaxTicks = 0x2bca2875f4373fffL;
			static const int MillisPerDay = 0x5265c00;
			static const int MillisPerHour = 0x36ee80;
			static const int MillisPerMinute = 0xea60;
			static const int MillisPerSecond = 0x3e8;
			static  const __int64 MinTicks = 0L;
			static const double OADateMaxAsDouble = 2958466.0;
			static const double OADateMinAsDouble = -657435.0;
			static const __int64 OADateMinAsTicks = 0x6efdddaec64000L;
			static const __int64 TicksCeiling = 0x4000000000000000L;
			static const unsigned __int64 TicksMask = 0x3fffffffffffffffL;
			static const __int64 TicksPerDay = 0xc92a69c000L;
			static const __int64 TicksPerHour = 0x861c46800L;
			static const __int64 TicksPerMillisecond = 0x2710L;
			static const __int64 TicksPerMinute = 0x23c34600L;
			static const __int64 TicksPerSecond = 0x989680L;
#pragma endregion

			/*--------------------------------------------------------------------------------
			* Private Member Declarations
			*------------------------------------------------------------------------------*/
#pragma region Private Member Declarations
			unsigned __int64	_dateData;
			static array<int>^	_daysToMonth365;
			static array<int>^	_daysToMonth366;
			bool				_isNull;
			bool				_isEmpty;

			static int	InternalCompare(__int64 leftTicks, __int64 rightTicks);
#pragma endregion

			/// <summary>
			/// Initializes a new instance of the structure.
			/// </summary>
			/// <remarks>
			/// This is the static value type constructor.
			/// </remarks>
			static SmartDateTime();

		};
	}
}