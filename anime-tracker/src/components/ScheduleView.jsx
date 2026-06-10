import { useState, useEffect } from 'react';
import { getSchedule } from '../api/jikan';
import AnimeGrid from './AnimeGrid';
import styles from './ScheduleView.module.css';

const DAYS = [
  { id: 'monday', label: 'Lunedì' },
  { id: 'tuesday', label: 'Martedì' },
  { id: 'wednesday', label: 'Mercoledì' },
  { id: 'thursday', label: 'Giovedì' },
  { id: 'friday', label: 'Venerdì' },
  { id: 'saturday', label: 'Sabato' },
  { id: 'sunday', label: 'Domenica' },
];

const JS_TO_API = ['sunday', 'monday', 'tuesday', 'wednesday', 'thursday', 'friday', 'saturday'];

function getTodayId() {
  return JS_TO_API[new Date().getDay()];
}

export default function ScheduleView({ onCardClick }) {
  const todayId = getTodayId();
  const [activeDay, setActiveDay] = useState(todayId);
  const [data, setData] = useState({});
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);

  useEffect(() => {
    if (data[activeDay]) return;
    setLoading(true);
    setError(null);
    getSchedule(activeDay)
      .then((d) => setData((prev) => ({ ...prev, [activeDay]: d.data || [] })))
      .catch((e) => setError(e.message))
      .finally(() => setLoading(false));
  }, [activeDay, data]);

  return (
    <div>
      <div className={styles.dayNav}>
        {DAYS.map((d) => (
          <button
            key={d.id}
            className={`${styles.dayBtn} ${activeDay === d.id ? styles.dayActive : ''} ${
              d.id === todayId ? styles.today : ''
            }`}
            onClick={() => setActiveDay(d.id)}
          >
            {d.label}
            {d.id === todayId && <span className={styles.todayDot}>●</span>}
          </button>
        ))}
      </div>

      <AnimeGrid
        items={data[activeDay]}
        loading={loading}
        error={error}
        onCardClick={onCardClick}
        emptyMsg="Nessun anime in palinsesto per questo giorno."
      />
    </div>
  );
}
