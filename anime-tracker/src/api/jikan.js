import axios from 'axios';

const BASE = 'https://api.jikan.moe/v4';

const api = axios.create({ baseURL: BASE });

const delay = (ms) => new Promise((r) => setTimeout(r, ms));

async function withRetry(fn, retries = 3) {
  for (let i = 0; i < retries; i++) {
    try {
      return await fn();
    } catch (err) {
      if (err.response?.status === 429 && i < retries - 1) {
        await delay(1500 * (i + 1));
      } else {
        throw err;
      }
    }
  }
}

export const getCurrentSeason = (page = 1) =>
  withRetry(() => api.get('/seasons/now', { params: { page, limit: 25 } })).then(
    (r) => r.data
  );

export const getSchedule = (day) =>
  withRetry(() => api.get('/schedules', { params: { filter: day, limit: 25 } })).then(
    (r) => r.data
  );

export const searchAnime = (q, page = 1) =>
  withRetry(() =>
    api.get('/anime', { params: { q, page, limit: 20, order_by: 'score', sort: 'desc' } })
  ).then((r) => r.data);

export const getAnimeById = (id) =>
  withRetry(() => api.get(`/anime/${id}/full`)).then((r) => r.data);

export const getTopAiring = (page = 1) =>
  withRetry(() =>
    api.get('/top/anime', { params: { filter: 'airing', page, limit: 25 } })
  ).then((r) => r.data);
